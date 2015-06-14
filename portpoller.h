#ifndef PORTPOLLER_H
#define PORTPOLLER_H

#include <QObject>

class PortPoller : public QObject
{
    Q_OBJECT

    quint16 vid_, pid_;
    bool portPresent_ = false;
    int tmrId_ = -1;

    void checkPort();
public:
    PortPoller(quint16 vid, quint16 pid);

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
