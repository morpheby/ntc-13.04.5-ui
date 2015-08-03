#include "configstore.h"
#include <QApplication>

ConfigStore& ConfigStore::instance()
{
    static ConfigStore instance;
    return instance;
}

ConfigStore::ConfigStore()
{
    m_settingsFile = QApplication::applicationDirPath() + "/NTC_13_04_5.ini";
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    m_minForce = settings.value("MIN_FORCE", MIN_FORCE).toDouble();
    m_maxForce = settings.value("MAX_FORCE", MAX_FORCE).toDouble();
    m_dMin = settings.value("D_MIN", D_MIN).toDouble();
    m_dMax = settings.value("D_MAX", D_MAX).toDouble();
    m_timeRange = settings.value("TIME_RANGE", TIME_RANGE).toDouble();
    m_powerThreshold = settings.value("DRIVER_POWER_SENSIVITY_THRESHOLD", DRIVER_POWER_SENSIVITY_THRESHOLD).toInt();
}

ConfigStore::~ConfigStore()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.setValue("MIN_FORCE", m_minForce);
    settings.setValue("MAX_FORCE", m_maxForce);
    settings.setValue("D_MIN", m_dMin);
    settings.setValue("D_MAX", m_dMax);
    settings.setValue("TIME_RANGE", m_timeRange);
    settings.setValue("DRIVER_POWER_SENSIVITY_THRESHOLD", m_powerThreshold);
}

