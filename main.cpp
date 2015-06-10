#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>

#include "serial/Log.h"
#include "serial/Logger.h"

void logging_terminate_handler() __attribute__ ((__noreturn__));

void logging_terminate_handler() {
    util::Logger::getInstance()->logException("Terminate called on active thread. Aborting...");
    std::abort();
}

int main(int argc, char *argv[])
{

    auto logger = std::make_shared<util::Logger>();
    logger->addLog(std::make_shared<util::LogFile>("ntpc-comm", util::LogSeverity::TRACE, "ntpc-comm.log"));
    logger->addLog(std::make_shared<util::LogStream>("ntpc-comm", util::LogSeverity::ERROR, std::cerr.rdbuf()));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
