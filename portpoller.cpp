#include "portpoller.h"
#include <QSerialPortInfo>


PortPoller::PortPoller(quint16 vid, quint16 pid) :
    vid_{vid}, pid_{pid} {
}

void PortPoller::start() {
    tmrId_ = this->startTimer(2000);
}

void PortPoller::stop() {
    if (tmrId_ > 0) {
        this->killTimer(tmrId_);
    }
    tmrId_ = -1;
}

void PortPoller::timerEvent(QTimerEvent *event) {
    this->checkPort();
}

void PortPoller::checkPort() {
    auto portList = QSerialPortInfo::availablePorts();
    bool portNotFound = true;
    for (auto &port : portList) {
        if (port.vendorIdentifier() == vid_ &&
            port.productIdentifier() == pid_) {;
            if (!portPresent_) {
                setConnected(true);
                emit foundPort(port.systemLocation());
            }
            portNotFound = false;
            break;
        }
    }
    if (portNotFound && portPresent_) {
        setConnected(false);
        emit portDisconnected();
    }
}

void PortPoller::setConnected(bool connected) {
    std::lock_guard<std::mutex> __guard(lock_);
    portPresent_ = connected;
}

bool PortPoller::isConnected() const {
    return portPresent_;
}
