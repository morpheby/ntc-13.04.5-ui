#ifndef PDPOLLER_H
#define PDPOLLER_H

#include <QObject>
#include <memory>
#include "pdapi.h"

class PdPoller : public QObject
{
    Q_OBJECT

    std::shared_ptr<PD::PdApi> apiConnection_;
    int tmrId_ = -1;
public:
    PdPoller(const std::shared_ptr<PD::PdApi> &pdApiConnection);
    PdPoller();

    void setApiConnection(const std::shared_ptr<PD::PdApi> &apiConnection);
    std::shared_ptr<PD::PdApi> apiConnection() const;
public slots:
    void poll();
    void deviceConnected(const std::shared_ptr<PD::PdApi> &apiConnection);
    void deviceDisconnected();
signals:
    void dataReceived(double d_in, double adc);
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // PDPOLLER_H
