#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pdpoller.h"
#include "serial/Logger.h"
#include "qcustomplot.h"
#include <algorithm>
#include "alglib/interpolation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Waiting for connection…"));

    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->xAxis->setLabel(tr("d, cm"));
    ui->plot->yAxis->setLabel(tr("F, N"));

    ui->plot->xAxis->setRange(0, 0.001);
    ui->plot->yAxis->setRange(0, 0.001);

    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsLine);
//    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
//    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    this->startTimer(1000);

    synchronizePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pollDataUpdated(double d_in, double adc) {
    if (recording_) {
        dataXPlot_.push_back(d_in/100.);
        dataYPlot_.push_back(adc);
        util::Logger::getInstance()->trace("Added to plot: " +
                                           std::to_string(d_in) + ":" +
                                           std::to_string(adc));
        this->synchronizePlot();
    }
}

void MainWindow::didConnect() {
    statusBar()->showMessage(tr("Device connected"));
}

void MainWindow::didLostConnection() {
    statusBar()->showMessage(tr("Waiting for connection…"));
}

void MainWindow::didPushStart() {
    recording_ = true;
    util::Logger::getInstance()->log("Plot recording started");
}

void MainWindow::didPushStop() {
    recording_ = false;
    util::Logger::getInstance()->log("Plot recording stopped");
}

void MainWindow::didPushClear() {
    dataXPlot_.clear();
    dataYPlot_.clear();
    this->synchronizePlot();
    util::Logger::getInstance()->log("Plot cleared");
}

void MainWindow::synchronizePlot() {
    auto plot = ui->plot;
    auto graph = plot->graph(0);

    graph->clearData();
    graph->setData(dataXPlot_, dataYPlot_);

    plot->replot();
}

std::pair<QVector<double>, QVector<double>> MainWindow::buildSplinePoints() {
    if (dataXPlot_.count() == 0 || dataYPlot_.count() == 0) {
        return {};
    }

    // First, group points

    std::vector<std::pair<double, double>> values(dataXPlot_.size());
    std::transform(dataXPlot_.begin(), dataXPlot_.end(), dataYPlot_.begin(),
                   values.begin(), [](double x, double y) {
        return std::pair<double, double>(x, y);
    });
    std::sort(values.begin(), values.end(),
              [](const std::pair<double, double> &lhs,
              const std::pair<double, double> &rhs) {
        return lhs.first < rhs.first;
    });
    std::vector<std::pair<double, double>> meanValues = {{0,0}};
    double lastX = values.front().first;
    int count = 0;
    for (auto &p : values) {
        if (p.first != lastX) {
            auto &r = meanValues.back();
            r.first /= count;
            r.second /= count;
            count = 0;
            lastX = p.first;
            meanValues.push_back({0, 0});
        }
        auto &r = meanValues.back();
        r.first += p.first;
        r.second += p.second;
        ++count;
    }
    {
        auto &r = meanValues.back();
        r.first /= count;
        r.second /= count;
    }

    if (meanValues.size() < 2) {
        return {};
    }


    std::vector<double> xs_v(meanValues.size());
    std::vector<double> ys_v(meanValues.size());
    std::transform(meanValues.begin(), meanValues.end(),
                   xs_v.begin(), [](const std::pair<double, double> &x) {
        return x.first;
    });
    std::transform(meanValues.begin(), meanValues.end(),
                   ys_v.begin(), [](const std::pair<double, double> &x) {
        return x.second;
    });


    auto xs = alglib::real_1d_array();
    xs.setcontent(xs_v.size(), xs_v.data());
    auto ys = alglib::real_1d_array();
    ys.setcontent(ys_v.size(), ys_v.data());

    alglib::spline1dinterpolant c;
    alglib::spline1dbuildakima(xs, ys, c);

    auto xrange = std::minmax_element(xs_v.begin(), xs_v.end());

    const int pointCount = 100;

    QVector<double> xr(pointCount);
    QVector<double> yr(pointCount);

    for (int i = 0; i < pointCount; ++i) {
        xr[i] = *xrange.first +
                ((double)i) * (*xrange.second-*xrange.first)/((double)pointCount);
        yr[i] = alglib::spline1dcalc(c, xr[i]);
    }

    return {xr,yr};
}

void MainWindow::timerEvent(QTimerEvent *event) {
    auto s = this->buildSplinePoints();
    auto plot = ui->plot;
    auto graph = plot->graph(1);
    const auto &xr = s.first;
    const auto &yr = s.second;
    graph->setData(s.first, s.second);

    auto xrange_1 = std::minmax_element(dataXPlot_.begin(), dataXPlot_.end());
    auto yrange_1 = std::minmax_element(dataYPlot_.begin(), dataYPlot_.end());
    auto xrange_2 = std::minmax_element(xr.begin(), xr.end());
    auto yrange_2 = std::minmax_element(yr.begin(), yr.end());


    auto xrange = std::make_pair(std::min(*xrange_1.first, *xrange_2.first),
                                 std::max(*xrange_1.second, *xrange_2.second));
    auto yrange = std::make_pair(std::min(*yrange_1.first, *yrange_2.first),
                                 std::max(*yrange_1.second, *yrange_2.second));

    plot->xAxis->setRange(xrange.first, xrange.second);
    plot->yAxis->setRange(yrange.first, yrange.second);

    plot->replot();
}

void MainWindow::didPushSet() {
    emit requestControllerSet();
}
