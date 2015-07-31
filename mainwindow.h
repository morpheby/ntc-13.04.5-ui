#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "pdpoller.h"
#include <vector>
#include <utility>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<double> dataDPlot_;
    QVector<double> dataFPlot_;
    QVector<double> dataTPlot_;
    bool recording_ = false;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent * event) override;

public slots:
    void pollDataUpdated(double d_in, double adc);
    void didConnect();
    void didLostConnection();

private slots:
    void didPushStart();
    void didPushUp();
    void didPushDown();
    void didChangePower(int power);
    void didPushStop();
    void didPushClear();
    void didPushSet();

    void on_btnSave_clicked();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void requestControllerSet();
    void requestSetPower(int value);
    void requestSetDirection(bool up);
    void requestStart();
    void requestStop();

private:
    Ui::MainWindow *ui;
    QTime time;
    bool haveUnsavedData;

    void synchronizePlot();
    void clear();
    std::pair<QVector<double>, QVector<double>> buildSplinePoints(QVector<double> &xData, QVector<double> &yData);
};

#endif // MAINWINDOW_H
