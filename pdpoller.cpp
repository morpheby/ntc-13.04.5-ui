#include "pdpoller.h"
#include <errno.h>
#include "serial/Logger.h"

PdPoller::PdPoller(const std::shared_ptr<PD::PdApi> &apiConnection) :
    apiConnection_{apiConnection} {
    tmrId_ = this->startTimer(300);
}

PdPoller::PdPoller() : apiConnection_{nullptr} {

}

void PdPoller::poll() {
    try {
        int d_in = apiConnection_->readRegister(PD::Registers::D_In);
        util::Logger::getInstance()->log("Received d_in: " + std::to_string(d_in));

        int adc = apiConnection_->readRegister(PD::Registers::ADC1);
        util::Logger::getInstance()->log("Received adc: " + std::to_string(adc));

        emit dataReceived(d_in, adc);
    } catch (const util::posix_error_exception &e) {
        util::Logger::getInstance()->logException(e);
        if (e.getErrno() == ETIMEDOUT) {
            // Log and silently ignore
        }
        try {
            apiConnection_->connection().flush();
        } catch (const std::exception &e) {
            util::Logger::getInstance()->logException(e);
        }
    }
}

void PdPoller::timerEvent(QTimerEvent *event) {
    this->poll();
}

void PdPoller::deviceConnected(const std::shared_ptr<PD::PdApi> &apiConnection) {
    this->setApiConnection(apiConnection);
    tmrId_ = this->startTimer(300);
}

void PdPoller::deviceDisconnected() {
    if (tmrId_ > 0) {
        this->killTimer(tmrId_);
    }
    tmrId_ = -1;

    this->setApiConnection(nullptr);
}

void PdPoller::setApiConnection(const std::shared_ptr<PD::PdApi> &apiConnection) {
    apiConnection_ = apiConnection;
}

std::shared_ptr<PD::PdApi> PdPoller::apiConnection() const {
    return apiConnection_;
}
