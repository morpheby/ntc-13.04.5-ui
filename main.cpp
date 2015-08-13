#include <QApplication>
#include <QThread>
#include <QTranslator>
#include <QLibraryInfo>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>

#include "serial/Log.h"
#include "serial/Logger.h"
#include "pdapi.h"
#include "modbusdriver.h"
#include "mainwindow.h"
#include "pdpoller.h"
#include "portpoller.h"
#include "configstore.h"

#define DEVICE_VID  0x0403
#define DEVICE_PID  0x6001
#define DEVICE_ADDR 0x01

void logging_terminate_handler() __attribute__ ((__noreturn__));

void logging_terminate_handler() {
    util::Logger::getInstance()->logException("Terminate called on active thread. Aborting...");
    std::abort();
}

int main(int argc, char *argv[])
{

    auto logger = std::make_shared<util::Logger>();
#ifdef DEBUG
    logger->addLog(std::make_shared<util::LogFile>("ntpc-comm", util::LogSeverity::severity_TRACE, "ntpc-comm.log"));
    logger->addLog(std::make_shared<util::LogStream>("ntpc-comm", util::LogSeverity::severity_ERROR, std::cerr.rdbuf()));
#else
//    logger->addLog(std::make_shared<util::LogFile>("ntpc-comm", util::LogSeverity::severity_INFO, "ntpc-comm.log"));
    logger->addLog(std::make_shared<util::LogStream>("ntpc-comm", util::LogSeverity::severity_ERROR, std::cerr.rdbuf()));
#endif

//    auto d_in = api.readRegister(PD::Registers::D_In);
//
//    logger->log("D_In: " + std::to_string(d_in));

    QApplication app(argc, argv);

    ConfigStore config = ConfigStore::instance();
    int minPower = config.powerThreshold();
    int powerCoef = (4096 - minPower)/100;

    std::shared_ptr<ModBus::ModBusDriver> driver = nullptr;
    std::shared_ptr<PD::PdApi> api = nullptr;

    QThread pollerThread;
    PdPoller *pdpoller = new PdPoller();
    PortPoller *portpoller = new PortPoller(DEVICE_VID, DEVICE_PID);
    portpoller->moveToThread(&pollerThread);
    pdpoller->moveToThread(&pollerThread);


    // Localization
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("ntc_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    MainWindow window;

    QObject::connect(&pollerThread, &QThread::finished, pdpoller, &QObject::deleteLater);
    QObject::connect(&pollerThread, &QThread::finished, portpoller, &QObject::deleteLater);
    QObject::connect(&pollerThread, &QThread::started, portpoller, &PortPoller::start);


    QObject::connect(portpoller, &PortPoller::foundPort, pdpoller, [&](QString portPath) {
        auto portPathUtf8 = std::string(portPath.toUtf8().data());
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)
        // Substitute cu for tty, since libmodbus doesn't support cu devices
        auto s_location = portPathUtf8.find("cu.");
        portPathUtf8 =
                    portPathUtf8.substr(0, s_location) +
                    "tty." +
                    portPathUtf8.substr(s_location+3, portPathUtf8.size()-s_location-3);

#endif
        try {
            driver = std::make_shared<ModBus::ModBusDriver>(portPathUtf8.c_str());
            auto connection = driver->connectionToDevice(DEVICE_ADDR);
            api = std::make_shared<PD::PdApi>(connection);
            pdpoller->deviceConnected(api);
        } catch (const std::exception &e) {
            driver = nullptr;
            logger->logException(e);
            portpoller->setConnected(false);
        }
    });
    QObject::connect(portpoller, &PortPoller::portDisconnected, pdpoller, [&]() {
        driver = nullptr;
    });
    QObject::connect(portpoller, &PortPoller::portDisconnected, pdpoller, &PdPoller::deviceDisconnected);

    pollerThread.start();

    QObject::connect(pdpoller, &PdPoller::dataReceived, &window, &MainWindow::pollDataUpdated);
    QObject::connect(pdpoller, &PdPoller::didLostConnection, portpoller, [&](){
        portpoller->setConnected(false);
    });
    QObject::connect(pdpoller, &PdPoller::didConnect, &window, &MainWindow::didConnect);

    QObject::connect(portpoller, &PortPoller::portDisconnected, &window, &MainWindow::didLostConnection);
    QObject::connect(pdpoller, &PdPoller::didLostConnection, &window, &MainWindow::didLostConnection);

    QObject::connect(&window, &MainWindow::requestControllerSet, pdpoller, [&]() {
        try {
            api->writeRegister<float>(PD::Registers::K0, 1.0);
            api->writeRegister<float>(PD::Registers::K1, 1.0);
            api->writeRegister<float>(PD::Registers::K2, 1.0);
            api->writeRegister(PD::Registers::OFS_ADC1, 0);
            api->writeRegister(PD::Registers::FLASH_WR, 1);
        } catch(...) {

        }
    });
    QObject::connect(&window, &MainWindow::requestSetPower, pdpoller, [&](int power) {
        try {
            int value = 0;
            if(power > 0) {
                value = minPower+power*powerCoef;
            }
            api->writeRegister<int>(PD::Registers::Power0, value);
        } catch(...) {
        }
    });

    QObject::connect(&window, &MainWindow::requestMoveUp, pdpoller, [&]() {
        try {
            api->writeRegister<int>(PD::Registers::Control0, 0x01);
            window.setDriverStarted(true);
        } catch(...) {
        }
    });
    QObject::connect(&window, &MainWindow::requestMoveDown, pdpoller, [&]() {
        try {
            api->writeRegister<int>(PD::Registers::Control0, 0x02);
            window.setDriverStarted(true);
        } catch(...) {
        }
    });
    QObject::connect(&window, &MainWindow::requestStop, pdpoller, [&]() {
        try {
            api->writeRegister<int>(PD::Registers::Control0, 0x00);
            window.setDriverStarted(false);
        } catch(...) {
        }
    });

    window.show();

    int retval = app.exec();

    pollerThread.quit();

    return retval;
}
