#ifndef PORTPOLLER_H
#define PORTPOLLER_H

#include <QObject>
#include <mutex>

class PortPoller : public QObject
{
    Q_OBJECT

    quint16 vid_, pid_;
    bool portPresent_ = false;
    int tmrId_ = -1;
    std::mutex lock_;

    void checkPort();
public:
    PortPoller(quint16 vid, quint16 pid);
    void setConnected(bool connected);
    bool isConnected() const;

public slots:
    void start();
    void stop();

signals:
    void foundPort(QString portPath);
    void portDisconnected();

protected:
    void timerEvent(QTimerEvent *event);

};

#endif // PORTPOLLER_H
