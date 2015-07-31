#ifndef CONFIGSTORE_H
#define CONFIGSTORE_H

#include <QString>
#include <QSettings>

static const double MIN_FORCE = 0;
static const double MAX_FORCE = 5000;
static const double D_MIN = 0;
static const double D_MAX = 100;
static const double TIME_RANGE = 60;

class ConfigStore
{
public:
    ~ConfigStore();
    static ConfigStore& instance();
    double minForce(){return m_minForce;}
    double maxForce(){return m_maxForce;}
    double dMin(){return m_dMin;}
    double dMax(){return m_dMax;}
    double timeRange(){return m_timeRange;}
private:
    explicit ConfigStore();
    QString m_settingsFile;
    double m_minForce;
    double m_maxForce;
    double m_dMin;
    double m_dMax;
    double m_timeRange;
};

#endif // CONFIGSTORE_H
