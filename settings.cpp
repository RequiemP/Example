#include "settings.h"

Settings::Settings() { }

Settings::~Settings() { }

void Settings::setSize(QSize const& size) {
   m_size = size;
   QSettings st(QSettings::IniFormat, QSettings::UserScope, COMPANY_NAME, PRODUCT_NAME);
   st.setValue(SettingsStrings::WINDOW + "/" + SettingsStrings::WINDOW_SIZE, m_size);
}

void Settings::setPos(QPoint const& pos) {
    m_pos = pos;
    QSettings st(QSettings::IniFormat, QSettings::UserScope, COMPANY_NAME, PRODUCT_NAME);
    st.setValue(SettingsStrings::WINDOW + "/" + SettingsStrings::WINDOW_POSITION, m_pos);
}

void Settings::setCmbCom(int const& cmbCom) {
    m_cmbCom = cmbCom;
    QSettings st(QSettings::IniFormat, QSettings::UserScope, COMPANY_NAME, PRODUCT_NAME);
    st.setValue(SettingsStrings::WINDOW + "/" + SettingsStrings::PORT, m_cmbCom);
}

void Settings::setLastDir(QString const& lastDir) {
    m_lastDir = lastDir;
    QSettings st(QSettings::IniFormat, QSettings::UserScope, COMPANY_NAME, PRODUCT_NAME);
    st.setValue(SettingsStrings::WINDOW + "/" + SettingsStrings::LAST_DIR, m_lastDir);
}

QSize Settings::getSize() const {
    return m_size;
}

QPoint Settings::getPos() const {
    return m_pos;
}

int Settings::getCmbCom() const {
    return m_cmbCom;
}

QString Settings::getLastDir() const {
     return m_lastDir;
}

void Settings::loadSettingsFromIni() {
    QSettings st(QSettings::IniFormat, QSettings::UserScope, COMPANY_NAME, PRODUCT_NAME);
    st.beginGroup(SettingsStrings::WINDOW);
    m_size = st.value(SettingsStrings::WINDOW_SIZE, QSize(415, 530)).toSize();
    m_pos = st.value(SettingsStrings::WINDOW_POSITION, QPoint(400, 400)).toPoint();
    m_cmbCom = st.value(SettingsStrings::PORT, int(-1)).toInt();
    m_lastDir = st.value(SettingsStrings::LAST_DIR, "").toString();
    st.endGroup();
}

Settings* Settings::m_settings = nullptr;

Settings* Settings::getInstance() {
    if(m_settings == nullptr) {
        m_settings = new Settings;
        m_settings->loadSettingsFromIni();
    }
    return m_settings;
}
