#ifndef MAINWINDOW
#define MAINWINDOW

#include <QMainWindow>
#include <QtWidgets>
#include <QHash>
#include <QPair>
#include <QVector>

#include "settings.h"
#include "serialcom.h"
#include "timeouttimer.h"
#include "mediatorgui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_boxSerial_currentIndexChanged(int index) const;
    void on_btApply_pressed() const;
    void on_btStart_pressed() const;
    void on_btFlash_pressed();
    void bytesWritten(quint64 const &bytes);
    void flashFinished();
    void checkFlashData();
    void receiveUartData();

private:
    Ui::MainWindow *ui;
    SerialCom* m_serial;
    size_t m_dataCount;
    size_t m_ramLength;
    size_t m_rowIndex;
    size_t m_columnCount;
    quint64 m_flashBytesSent;
    MediatorGui* m_mediator;
    TimeoutTimer* m_flashTimer;
    void initMediator();
    void setUpOnExchangeEnd() const;
    void setUpOnExchangeStart() const;
    void initialConnect() const;
    void initGui();
    void loadSettingsToIni() const;
    void connectSerialPort() const;
    void start() const;
    void serialSettings(QString const &portName) const;
    void displayResult(QString const& name, QString const& result, bool const& check) const;
};

#endif // MAINWINDOW

