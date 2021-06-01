#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_serial = new SerialCom(this); //осторожно: raw указатели :)
    m_flashTimer = new TimeoutTimer(20, this);
    m_mediator = new MediatorGui(this);

    this->initMediator();
    this->initGui();
}

void MainWindow::initGui() {
    this->setWindowTitle("Evaluation board");
    this->initialConnect();

    Settings::getInstance()->loadSettingsFromIni();
    move(Settings::getInstance()->getPos());
    resize(Settings::getInstance()->getSize());

    m_mediator->notifyTable(KeyWords::configTable);
    m_mediator->notifyCmbCom(KeyWords::start);
    m_mediator->notifyBtStBar(KeyWords::btDisable);
}

void MainWindow::loadSettingsToIni() const {
    Settings::getInstance()->setPos(pos());
    Settings::getInstance()->setSize(size());
    m_mediator->notifyCmbCom(KeyWords::loadSettings);
}

MainWindow::~MainWindow() {
    this->loadSettingsToIni();
    delete ui;
    delete m_serial;
    delete m_flashTimer;
    delete m_mediator;
}

void MainWindow::connectSerialPort() const
{
    QString portName = m_mediator->getCmbText();
    this->serialSettings(portName);

    if (m_serial->open(QIODevice::ReadWrite)) {
        m_mediator->notifyBtStBar(KeyWords::portOpen, portName);
    } else if (m_serial->isOpen()) {
        m_serial->close();
        m_mediator->notifyBtStBar(KeyWords::portClose);
    } else if (m_mediator->getCmbCount() == 0) {
        m_mediator->notifyBtStBar(KeyWords::noPorts);
    } else {
        m_mediator->notifyBtStBar(KeyWords::portBusy);
    }
}

void MainWindow::checkFlashData() {
    m_serial->receiveBytesFlash();
    m_flashTimer->startTimer();
    ++m_dataCount;
    m_mediator->notifyBtStBar(KeyWords::loadFlash, QString::number(m_dataCount));
}

void MainWindow::receiveUartData() {
    m_serial->recieveData();
}

void MainWindow::initMediator() {
    m_mediator->setBtStart(ui->btStart);
    m_mediator->setBtApply(ui->btApply);
    m_mediator->setBtFlash(ui->btFlash);
    m_mediator->setCmbCom(ui->boxSerial);
    m_mediator->setLbDesc(ui->lbDescription);
    m_mediator->setLbManuf(ui->lbManufacturer);
    m_mediator->setStBar(ui->statusBar);
    m_mediator->setTabWidget(ui->tabWidget);
    m_mediator->setTableResult(ui->tableResult);
    m_mediator->setTbResult(ui->tbResult);
    m_serial->setMediator(m_mediator);
}

void MainWindow::bytesWritten(quint64 const& bytes) {
    m_flashBytesSent = bytes;
}

void MainWindow::flashFinished() {
    m_flashTimer->stopTimer();
    m_serial->setBaudRate(QSerialPort::Baud9600);
    this->setUpOnExchangeEnd();
    if  (m_serial->validCheckFlash()) {
        m_mediator->notifyBtStBar(KeyWords::finishFlash, QString::number(m_flashBytesSent));
    } else {
        m_serial->clear();
        m_mediator->notifyBtStBar(KeyWords::failFlash);
    }
    m_flashBytesSent = 0;
}

void MainWindow::setUpOnExchangeEnd() const {
    QObject::disconnect(m_serial, &SerialCom::readyRead, this, &MainWindow::checkFlashData);
    QObject::disconnect(m_serial, &SerialCom::bytesWritten, this, &MainWindow::bytesWritten);
    QObject::connect(m_serial, &SerialCom::readyRead, this, &MainWindow::receiveUartData);
}

void MainWindow::setUpOnExchangeStart() const {
    QObject::connect(m_serial, &SerialCom::readyRead, this, &MainWindow::checkFlashData);
    QObject::connect(m_serial, &SerialCom::bytesWritten, this, &MainWindow::bytesWritten);
    QObject::disconnect(m_serial, &SerialCom::readyRead, this, &MainWindow::receiveUartData);
}

void MainWindow::initialConnect() const {
    QObject::connect(m_serial, &SerialCom::readyRead, this, &MainWindow::receiveUartData);
    QObject::connect(m_flashTimer, &TimeoutTimer::timeoutSignal, this, &MainWindow::flashFinished);
    QObject::connect(m_serial, &SerialCom::checkReady, this, &MainWindow::displayResult);
}

void MainWindow::start() const {
    m_mediator->notifyTable(KeyWords::resetTable);
    m_serial->startMessage();
    m_mediator->notifyBtStBar(KeyWords::dataSent);
}

void MainWindow::serialSettings(QString const& portName) const {
    m_serial->initSerial(portName);
}

void MainWindow::displayResult(const QString &name, const QString &result, const bool &check) const {
    m_mediator->notifyTable(KeyWords::fillTable, name, result, check);
}

void MainWindow::on_boxSerial_currentIndexChanged(int index) const {
    m_mediator->notifyCmbCom(KeyWords::indexChange, index);
}

void MainWindow::on_btApply_pressed() const {
    connectSerialPort();
}

void MainWindow::on_btStart_pressed() const {
    start();
}

void MainWindow::on_btFlash_pressed() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите файл"),
                                                     Settings::getInstance()->getLastDir(),
                                                     tr("Открыть бинарный файл (*.bin)"));
    if (QFileInfo::exists(fileName)) {
        this->setUpOnExchangeStart();
        Settings::getInstance()->setLastDir(QFileInfo(fileName).absoluteDir().absolutePath());
        m_dataCount = 0;
        m_ramLength = m_serial->flashFromFile(fileName);
        m_flashTimer->startTimer();
        m_mediator->notifyBtStBar(KeyWords::btDisable);
    }
}

