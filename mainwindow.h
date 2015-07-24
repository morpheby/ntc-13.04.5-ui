#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pdpoller.h"
#include <vector>
#include <utility>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<double> dataXPlot_;
    QVector<double> dataYPlot_;
    bool recording_ = false;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

    void synchronizePlot();
    std::pair<QVector<double>, QVector<double>> buildSplinePoints();
};

#endif // MAINWINDOW_H
