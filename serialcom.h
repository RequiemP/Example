#ifndef SerialCom_H
#define SerialCom_H

#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QHash>
#include <QPair>

#include "timeouttimer.h"
#include "moduleinfo.h"
#include "mediatorgui.h"

namespace CheckTable {
    QString const namePwm1      = "PWM1";
    QString const namePwm2      = "PWM2";
    QString const nameXintf1    = "XINTF1";
    QString const nameXintf2    = "XINTF2";
    QString const nameEcap1     = "ECAP1";
    QString const nameEcap2     = "ECAP2";
    QString const nameAdc       = "ADC";
    QString const nameCan       = "CAN";
    QString const nameI2c       = "I2C";
    QString const nameMcbsp     = "MCBSP";
    QString const nameEcapGpio  = "ECAP GPIO";

    char constexpr idStart          = '\xd1';
    char constexpr idPwm1           = '\xa1';
    char constexpr idPwm2           = '\xa2';
    char constexpr idXintf1         = '\xaa';
    char constexpr idXintf2         = '\x55';
    char constexpr idEcap1          = '\xa3';
    char constexpr idEcap2          = '\xa4';
    char constexpr idAdc            = '\xc1';
    char constexpr idCan            = '\xc2';
    char constexpr idI2c            = '\xc3';
    char constexpr idMcbsp          = '\xc4';
    char constexpr idEcapGpio       = '\xa5';
    char constexpr idEnd            = '\xd2';

    char constexpr valuePwm         = '\x3f';
    char constexpr valueXintf1      = '\xaa';
    char constexpr valueXintf2      = '\x55';
    char constexpr valueEcap        = '\x3f';
    char constexpr valueAdc         = '\xff';
    char constexpr valueCan         = '\x0f';
    char constexpr valueI2c         = '\x21';
    char constexpr valueMcbsp       = '\xbb';
    char constexpr valueEcapGpio    = '\x5a';

    int constexpr lenPwm            = 1;
    int constexpr lenXintf          = 3;
    int constexpr lenEcap           = 1;
    int constexpr lenAdc            = 2;
    int constexpr lenCan            = 1;
    int constexpr lenI2c            = 1;
    int constexpr lenMcbsp          = 1;
    int constexpr lenEcapGpio       = 1;
}

class MediatorGui;

class SerialCom : public QSerialPort {
    Q_OBJECT
public:
    explicit SerialCom(QObject* parent = nullptr);
    ~SerialCom();

    void recieveData();
    void fillCmb(QComboBox* const& cmb) const;
    void validCheck(QByteArray& byte) const; // TODO: закинуть в private
    int flashFromFile(QString const& fileName);
    void startMessage();
    void receiveBytesFlash();
    bool validCheckFlash();
    void setMediator(MediatorGui* const& mediator);
    void initSerial(QString const& portName);

signals:
    void checkReady(QString const& name, QString const& result, bool const& check) const;

private:
    TimeoutTimer* m_uartTimer;
    MediatorGui* m_mediatorSerial;
    QByteArray m_flashFile;
    QByteArray m_serialData;
    void validCheckInteralCycle(ModuleInfo const& pair_, QByteArray const& byte, int const& i) const;
    QString formatString(char const& byteChar) const;
    void uartTimeout();
    QHash<char, ModuleInfo> const m_validTable {{CheckTable::idPwm1, ModuleInfo(CheckTable::namePwm1, CheckTable::valuePwm, CheckTable::lenPwm)},
                                                 {CheckTable::idPwm2, ModuleInfo(CheckTable::namePwm2, CheckTable::valuePwm, CheckTable::lenPwm)},
                                                 {CheckTable::idXintf1, ModuleInfo(CheckTable::nameXintf1, CheckTable::valueXintf1, CheckTable::lenXintf)},
                                                 {CheckTable::idXintf2, ModuleInfo(CheckTable::nameXintf2, CheckTable::valueXintf2, CheckTable::lenXintf)},
                                                 {CheckTable::idEcap1, ModuleInfo(CheckTable::nameEcap1, CheckTable::valueEcap, CheckTable::lenEcap)},
                                                 {CheckTable::idEcap2, ModuleInfo(CheckTable::nameEcap2, CheckTable::valueEcap, CheckTable::lenEcap)},
                                                 {CheckTable::idAdc, ModuleInfo(CheckTable::nameAdc, CheckTable::valueAdc, CheckTable::lenAdc)},
                                                 {CheckTable::idCan, ModuleInfo(CheckTable::nameCan, CheckTable::valueCan, CheckTable::lenCan)},
                                                 {CheckTable::idI2c, ModuleInfo(CheckTable::nameI2c, CheckTable::valueI2c, CheckTable::lenI2c)},
                                                 {CheckTable::idMcbsp, ModuleInfo(CheckTable::nameMcbsp, CheckTable::valueMcbsp, CheckTable::lenMcbsp)},
                                                 {CheckTable::idEcapGpio, ModuleInfo(CheckTable::nameEcapGpio, CheckTable::valueEcapGpio, CheckTable::lenEcapGpio)}};
    struct PortInfoStruct {
        QString m_portName;
        QString m_portDescription;
        QString m_portManufacturer;
    };
};

#endif // SerialCom_H

