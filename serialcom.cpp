#include "serialcom.h"

#include <QDebug>

SerialCom::SerialCom(QObject* parent) : QSerialPort(parent) {
    m_uartTimer = new TimeoutTimer(1000, this);
    QObject::connect(m_uartTimer, &TimeoutTimer::timeoutSignal, this, &SerialCom::uartTimeout);
}

SerialCom::~SerialCom() {
    delete  m_uartTimer;
}

void SerialCom::fillCmb(QComboBox* const& cmb) const {
    cmb->clear();
    foreach (QSerialPortInfo const& info, QSerialPortInfo::availablePorts()) {
        SerialCom::PortInfoStruct infoPoint;
        QStringList list;
        infoPoint.m_portDescription = info.description();
        infoPoint.m_portManufacturer = info.manufacturer();
        list << info.portName()
             << (!infoPoint.m_portDescription.isEmpty() ? infoPoint.m_portDescription : QString())
             << (!infoPoint.m_portManufacturer.isEmpty() ? infoPoint.m_portManufacturer : QString());
        cmb->addItem(list.first(), list);
    }
}

void SerialCom::validCheck(QByteArray& byte) const { // итерация по айди
    for (int i = 1; i < byte.size() - 2;) {
        if (m_validTable.find(byte[i]) != m_validTable.end()) {
            ModuleInfo current = m_validTable[byte[i]];
            ++i;
            this->validCheckInteralCycle(current, byte, i);
            i += current.getDataLength();
        } else {
            QString errorMes = QString::number(i) + ", " + this->formatString(byte.at(i)).remove(4, 2);
            m_mediatorSerial->notifyBtStBar(KeyWords::wrongFormat, errorMes);
            byte.clear();
            return;
        }
    }
    byte.clear();
}

void SerialCom::validCheckInteralCycle(ModuleInfo const& info, QByteArray const& byte, int const& i) const { // итерация по данным
    bool checkFlag = 1;
    QString currString = "";
    for (int j = 0; j < info.getDataLength();) {
        currString += this->formatString(byte[i + j]);
        info.getData() == byte.at(i + j) ? 1 : checkFlag = 0;
        ++j;
    }
    currString.remove(currString.length() - 2, 2);
    emit this->checkReady(info.getName(), currString, checkFlag);
}

QString SerialCom::formatString(char const& byteChar) const {
    QString strBuf = QString("%1").arg(QChar(byteChar).unicode(), 0, 16).toUpper();
    return strBuf.length() < 2 ? "0x0" + strBuf + ", " : "0x" + strBuf + ", ";
}

void SerialCom::uartTimeout() {
    m_uartTimer->stopTimer();
    m_serialData.clear();
    m_mediatorSerial->notifyBtStBar(KeyWords::timeout);
}

int SerialCom::flashFromFile(QString const& fileName) {
    m_serialData = "";
    m_flashFile = "A"; // Для настройки бодрейта
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)) {
        this->setBaudRate(QSerialPort::Baud115200);
        m_flashFile.append(file.readAll());
        this->write(m_flashFile);
    }
    return m_flashFile.length();
}

void SerialCom::startMessage() {
    QByteArray start = "";
    start.push_back(CheckTable::idStart);
    this->write(start);
    start.clear();
}

void SerialCom::receiveBytesFlash() {
    m_serialData.push_back(this->readAll());
}

bool SerialCom::validCheckFlash() {
    bool result;
    result = m_serialData == m_flashFile ? true : false;
    m_serialData = m_flashFile = "";
    return result;
}

void SerialCom::setMediator(MediatorGui* const& mediator) {
    m_mediatorSerial = mediator;
}

void SerialCom::initSerial(const QString &portName) {
    this->setPortName(portName);
    this->setBaudRate(QSerialPort::Baud9600);
    this->setDataBits(QSerialPort::Data8);
    this->setParity(QSerialPort::NoParity);
    this->setStopBits(QSerialPort::OneStop);
    this->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialCom::recieveData() {
    m_uartTimer->startTimer();
    m_serialData.push_back(this->readAll());
    qDebug() << m_serialData;
    if (m_serialData.front() == CheckTable::idStart && m_serialData.back() == CheckTable::idEnd) {
        m_uartTimer->stopTimer();
        this->validCheck(m_serialData);
    }
}
