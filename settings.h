#ifndef Settings_H
#define Settings_H

#include <QSettings>
#include <QSize>
#include <QPoint>
#include <QString>

namespace SettingsStrings {
    QString const WINDOW = "window";
    QString const WINDOW_SIZE = "size";
    QString const WINDOW_POSITION = "position";
    QString const PORT = "port";
    QString const LAST_DIR = "lastDir";
}

class Settings {
public:
    static Settings* getInstance();
    Settings(const Settings&) = delete;
    Settings& operator= (const Settings&) = delete;
    void setSize(const QSize& size);
    void setPos(const QPoint& pos);
    void setCmbCom(const int& cmbCom);
    void setLastDir(const QString& lastDir);
    QSize getSize() const;
    QPoint getPos() const;
    int getCmbCom() const;
    QString getLastDir() const;
    void loadSettingsFromIni();

private:
    Settings();
    ~Settings();
    QSize m_size;
    QPoint m_pos;
    int m_cmbCom;
    QString m_lastDir;
    static Settings* m_settings;
};

#endif // Settings_H
