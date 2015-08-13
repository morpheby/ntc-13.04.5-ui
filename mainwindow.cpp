#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pdpoller.h"
#include "serial/Logger.h"
#include "qcustomplot.h"
#include <algorithm>
#include "alglib/interpolation.h"
#include "configstore.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QTextCursor>
#include <QTextTable>
#include <QCloseEvent>

static const int T_PRECISION = 1;
static const int D_PRECISION = 2;
static const int F_PRECISION = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Waiting for connection…"));
    ui->powerSlider->setValue(0);

    ConfigStore config = ConfigStore::instance();

    //F(d)
    ui->f_d_plot->addGraph();
    ui->f_d_plot->addGraph();
    ui->f_d_plot->xAxis->setLabel(tr("d, mm"));
    ui->f_d_plot->yAxis->setLabel(tr("F, N"));

    ui->f_d_plot->xAxis->setRange(config.dMin(), config.dMax());
    ui->f_d_plot->yAxis->setRange(config.minForce(), config.maxForce());

    ui->f_d_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->f_d_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->f_d_plot->graph(1)->setLineStyle(QCPGraph::lsLine);

    //d(F)
    ui->d_f_plot->addGraph();
    ui->d_f_plot->addGraph();
    ui->d_f_plot->xAxis->setLabel(tr("F, N"));
    ui->d_f_plot->yAxis->setLabel(tr("d, mm"));

    ui->d_f_plot->xAxis->setRange(config.minForce(), config.maxForce());
    ui->d_f_plot->yAxis->setRange(config.dMin(), config.dMax());

    ui->d_f_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->d_f_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->d_f_plot->graph(1)->setLineStyle(QCPGraph::lsLine);

    //d(t)
    ui->d_t_plot->addGraph();
    ui->d_t_plot->addGraph();
    ui->d_t_plot->xAxis->setLabel(tr("t, s"));
    ui->d_t_plot->yAxis->setLabel(tr("d, mm"));

    ui->d_t_plot->xAxis->setRange(0, config.timeRange());
    ui->d_t_plot->yAxis->setRange(config.dMin(), config.dMax());

    ui->d_t_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->d_t_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->d_t_plot->graph(1)->setLineStyle(QCPGraph::lsLine);

    //F(t)
    ui->f_t_plot->addGraph();
    ui->f_t_plot->addGraph();
    ui->f_t_plot->xAxis->setLabel(tr("t, s"));
    ui->f_t_plot->yAxis->setLabel(tr("F, N"));

    ui->f_t_plot->xAxis->setRange(0, config.timeRange());
    ui->f_t_plot->yAxis->setRange(config.dMin(), config.dMax());

    ui->f_t_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->f_t_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->f_t_plot->graph(1)->setLineStyle(QCPGraph::lsLine);

    this->startTimer(1000);

    synchronizePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pollDataUpdated(double d_in, double adc) {
    if(adc > MAX_FORCE) {
        didPushStop();
    }
    if (recording_) {
        dataDPlot_.push_back(d_in*0.01);
        dataFPlot_.push_back(adc);
        dataTPlot_.push_back(time.elapsed()*0.001);
        util::Logger::getInstance()->trace("Added to plot: " +
                                           std::to_string(d_in) + ":" +
                                           std::to_string(adc));
        this->synchronizePlot();
    }
}

void MainWindow::didConnect() {
    emit requestStop();
    emit requestSetPower(0);
    ui->powerSlider->setValue(0);
    ui->stopButton->setEnabled(true);

    ui->startButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->upButton->setEnabled(true);
    ui->powerSlider->setEnabled(true);
    ui->clearButton->setEnabled(true);
    statusBar()->showMessage(tr("Device connected"));
}

void MainWindow::didLostConnection() {
    ui->startButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->powerSlider->setEnabled(false);
    ui->btnSave->setEnabled(haveUnsavedData);
    ui->stopButton->setEnabled(false);
    recording_ = false;
    statusBar()->showMessage(tr("Waiting for connection…"));
}

void MainWindow::didPushStart() {
    bool startAllowed = !haveUnsavedData;

    if(haveUnsavedData)
    {
        QMessageBox msgBox(
                    QMessageBox::Question,
                    tr("Start a new experiment"),
                    tr("Are you sure you want to start a new experiment?\nAll unsaved data will be lost."),
                    QMessageBox::Yes | QMessageBox::No);

        msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        msgBox.setButtonText(QMessageBox::No, tr("No"));

        if (msgBox.exec() == QMessageBox::Yes) {
            startAllowed = true;
        }
    }

    if(startAllowed)
    {
        clear();
        recording_ = true;
        time.restart();
        haveUnsavedData = true;

        ui->startButton->setEnabled(false);
        ui->btnSave->setEnabled(false);
        ui->clearButton->setEnabled(false);
        ui->stopButton->setEnabled(true);

        util::Logger::getInstance()->log("Plot recording started");
    }
}

void MainWindow::didPushUp() {
    emit requestMoveUp();
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    util::Logger::getInstance()->log("Direction: Up, motor start");
}

void MainWindow::didPushDown() {
    emit requestMoveDown();
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    util::Logger::getInstance()->log("Direction: Down, motor start");
}

void MainWindow::didChangePower(int power) {
    emit requestSetPower(power);
    util::Logger::getInstance()->log("Power set to " + std::to_string(power));
}

void MainWindow::didPushStop() {
    recording_ = false;
    emit requestStop();

    ui->btnSave->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    util::Logger::getInstance()->log("Plot recording stopped");
}

void MainWindow::clear()
{
    dataDPlot_.clear();
    dataFPlot_.clear();
    dataTPlot_.clear();
    haveUnsavedData = false;
    this->synchronizePlot();
    util::Logger::getInstance()->log("Plot cleared");
}

void MainWindow::didPushClear() {
    bool clearAllowed = !haveUnsavedData;
    if(haveUnsavedData)
    {
        QMessageBox msgBox(
                    QMessageBox::Question,
                    tr("Clear results"),
                    tr("Are you sure you want to clear results?\nAll unsaved data will be lost."),
                    QMessageBox::Yes | QMessageBox::No);

        msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        msgBox.setButtonText(QMessageBox::No, tr("No"));

        if (msgBox.exec() == QMessageBox::Yes) {
            clearAllowed = true;
        }
    }

    if(clearAllowed)
    {
        clear();
    }
}

void MainWindow::synchronizePlot() {
    //F(d) plot
    auto f_d_plot = ui->f_d_plot;
    auto f_d_graph = f_d_plot->graph(0);

    f_d_graph->clearData();
    f_d_graph->setData(dataDPlot_, dataFPlot_);

    f_d_plot->replot();

    //d(F) plot
    auto d_f_plot = ui->d_f_plot;
    auto d_f_graph = d_f_plot->graph(0);

    d_f_graph->clearData();
    d_f_graph->setData(dataFPlot_, dataDPlot_);

    d_f_plot->replot();

    //d(t) plot
    auto d_t_plot = ui->d_t_plot;
    auto d_t_graph = d_t_plot->graph(0);

    d_t_graph->clearData();
    d_t_graph->setData(dataTPlot_, dataDPlot_);

    d_t_plot->replot();


    //F(t) plot
    auto f_t_plot = ui->f_t_plot;
    auto f_t_graph = f_t_plot->graph(0);

    f_t_graph->clearData();
    f_t_graph->setData(dataTPlot_, dataFPlot_);

    f_t_plot->replot();
}

std::pair<QVector<double>, QVector<double>> MainWindow::buildSplinePoints(QVector<double> &xData, QVector<double> &yData) {
    if (xData.count() == 0 || yData.count() == 0) {
        return {};
    }

    // First, group points

    std::vector<std::pair<double, double>> values(xData.size());
    std::transform(xData.begin(), xData.end(), yData.begin(),
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
    Q_UNUSED(event)
    auto f_d_spline = this->buildSplinePoints(dataDPlot_, dataFPlot_);
    auto f_d_plot = ui->f_d_plot;
    auto f_d_spline_graph = f_d_plot->graph(1);
    const auto &xr = f_d_spline.first;
    const auto &yr = f_d_spline.second;
    f_d_spline_graph->setData(f_d_spline.first, f_d_spline.second);

    ConfigStore config = ConfigStore::instance();

    auto xrange_1 = std::minmax_element(dataDPlot_.begin(), dataDPlot_.end());
    auto yrange_1 = std::minmax_element(dataFPlot_.begin(), dataFPlot_.end());
    auto xrange_2 = std::minmax_element(xr.begin(), xr.end());
    auto yrange_2 = std::minmax_element(yr.begin(), yr.end());


    auto xrange = std::make_pair(std::min(*xrange_1.first, *xrange_2.first),
                                 std::max(*xrange_1.second, *xrange_2.second));
    auto yrange = std::make_pair(std::min(*yrange_1.first, *yrange_2.first),
                                 std::max(*yrange_1.second, *yrange_2.second));

    double minForce = std::min(config.minForce(), yrange.first);
    double maxForce = std::max(config.maxForce(), yrange.second);
    double dMin = std::min(config.dMin(), xrange.first);
    double dMax = std::max(config.dMax(), xrange.second);

    f_d_plot->xAxis->setRange(dMin, dMax);
    f_d_plot->yAxis->setRange(minForce, maxForce);

    f_d_plot->replot();

    auto d_f_plot = ui->d_f_plot;
    auto d_f_spline_graph = d_f_plot->graph(1);
    d_f_spline_graph->setData(f_d_spline.second, f_d_spline.first);

    d_f_plot->yAxis->setRange(dMin, dMax);
    d_f_plot->xAxis->setRange(minForce, maxForce);

    d_f_plot->replot();

    auto d_t_plot = ui->d_t_plot;
    auto d_t_spline_graph = d_t_plot->graph(1);
    auto d_t_spline = buildSplinePoints(dataTPlot_, dataDPlot_);
    d_t_spline_graph->setData(d_t_spline.first, d_t_spline.second);

    auto f_t_plot = ui->f_t_plot;
    auto f_t_spline_graph = f_t_plot->graph(1);
    auto f_t_spline = buildSplinePoints(dataTPlot_, dataFPlot_);
    f_t_spline_graph->setData(f_t_spline.first, f_t_spline.second);

    double timeRange = std::max(dataTPlot_.last(), config.timeRange());
    f_t_plot->xAxis->setRange(0, timeRange);
    d_t_plot->xAxis->setRange(0, timeRange);
    f_t_plot->yAxis->setRange(minForce, maxForce);
    d_t_plot->yAxis->setRange(dMin, dMax);

    d_t_plot->replot();
    f_t_plot->replot();
}

void MainWindow::didPushSet() {
    emit requestControllerSet();
}

void MainWindow::on_btnSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save report"), QDir::homePath().append("/").append(tr("NTC_13_04_2_report")), tr("HTML files (*.html);"));

    if(fileName.isEmpty())
    {
        return;
    }

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignHCenter);

    QTextCharFormat charFormat = cursor.charFormat();
    QFont font = charFormat.font();
    font.setPointSize(14);
    font.setBold(true);
    charFormat.setFont(font);

    cursor.setBlockCharFormat(charFormat);
    cursor.insertText(tr("NTC-13.04.2 Experiment report"));
    cursor.setBlockFormat(centerAlignment);

    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(7);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

    font.setPointSize(12);
    charFormat.setFont(font);

    cursor.insertTable(1, 3, tableFormat);

    cursor.setBlockCharFormat(charFormat);
    cursor.insertText(tr("Time, s"));
    cursor.setBlockFormat(centerAlignment);

    cursor.movePosition(QTextCursor::NextCell);
    cursor.setBlockCharFormat(charFormat);
    cursor.insertText(tr("d, mm"));
    cursor.setBlockFormat(centerAlignment);

    cursor.movePosition(QTextCursor::NextCell);
    cursor.setBlockCharFormat(charFormat);
    cursor.insertText(tr("F, N"));
    cursor.setBlockFormat(centerAlignment);

    font.setBold(false);
    charFormat.setFont(font);
    QTextTable *table = cursor.currentTable();

    QLocale locale;
    unsigned int count = dataDPlot_.size();

    for(unsigned int i = 0; i < count; ++i)
    {
        table->appendRows(1);

        cursor.movePosition(QTextCursor::PreviousRow);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockCharFormat(charFormat);
        cursor.insertText(locale.toString(dataTPlot_[i], 'f', T_PRECISION));
        cursor.setBlockFormat(centerAlignment);

        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockCharFormat(charFormat);
        cursor.insertText(locale.toString(dataDPlot_[i], 'f', D_PRECISION));
        cursor.setBlockFormat(centerAlignment);

        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockCharFormat(charFormat);
        cursor.insertText(locale.toString(dataFPlot_[i], 'f', F_PRECISION));
        cursor.setBlockFormat(centerAlignment);
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    QString imageFileName(fileName);
    int extensionIndex = imageFileName.lastIndexOf(".");
    imageFileName.remove(extensionIndex, imageFileName.length() - extensionIndex);

    QString f_d_graphImage = imageFileName + "_F(d).png";
    QString d_f_graphImage = imageFileName + "_d(F).png";
    QString d_t_graphImage = imageFileName + "_d(t).png";
    QString f_t_graphImage = imageFileName + "_F(t).png";

    auto f_d_plot = ui->f_d_plot;
    auto d_f_plot = ui->d_f_plot;
    auto f_t_plot = ui->f_t_plot;
    auto d_t_plot = ui->d_t_plot;

    f_d_plot->savePng(f_d_graphImage, 640, 480);
    d_f_plot->savePng(d_f_graphImage, 640, 480);
    f_t_plot->savePng(f_t_graphImage, 640, 480);
    d_t_plot->savePng(d_t_graphImage, 640, 480);

    f_d_graphImage.remove(0, imageFileName.lastIndexOf("/")+1);
    d_f_graphImage.remove(0, imageFileName.lastIndexOf("/")+1);
    d_t_graphImage.remove(0, imageFileName.lastIndexOf("/")+1);
    f_t_graphImage.remove(0, imageFileName.lastIndexOf("/")+1);

    cursor.insertImage(f_d_graphImage);
    cursor.insertImage(f_t_graphImage);
    cursor.insertImage(d_f_graphImage);
    cursor.insertImage(d_t_graphImage);

    QTextDocumentWriter writer(fileName);
    writer.setFormat("html");
    writer.write(&doc);

    haveUnsavedData = false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool exitAllowed = !haveUnsavedData;

    if(driverStarted) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("Motor was not stopped properly.\nAre you sure you want to exit?"), QMessageBox::Yes | QMessageBox::No);
        if(msgBox.exec()== QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }

    if(haveUnsavedData) {
        QMessageBox msgBox(
                     QMessageBox::Question,
                     tr("Exit confirmation"),
                     tr("Are you sure you want to exit?\nAll unsaved data will be lost."),
                     QMessageBox::Yes | QMessageBox::No);

         msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
         msgBox.setButtonText(QMessageBox::No, tr("No"));

         exitAllowed = (msgBox.exec() == QMessageBox::Yes);
    }

    if(exitAllowed)
    {
        event->accept();
    } else {
        event->ignore();
    }
}
