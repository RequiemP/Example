#include "mediatorgui.h"

MediatorGui::MediatorGui(QObject* parent) : QObject(parent) { }

void MediatorGui::setLbManuf(QLabel* const& lb) {
    m_lbManuf = lb;
}

void MediatorGui::setLbDesc(QLabel* const& lb) {
    m_lbDesc = lb;
}

void MediatorGui::setBtStart(QPushButton* const& bt) {
    m_btStart = bt;
}

void MediatorGui::setBtApply(QPushButton* const& bt) {
    m_btApply = bt;
}

void MediatorGui::setBtFlash(QPushButton* const& bt) {
    m_btFlash = bt;
}

void MediatorGui::setCmbCom(QComboBox* const& cmb) {
    m_cmb = cmb;
}

void MediatorGui::setStBar(QStatusBar* const& stBar) {
    m_stBar = stBar;
}

void MediatorGui::setTabWidget(QTabWidget* const& tabWidget) {
    m_tabWidget = tabWidget;
}

void MediatorGui::setTableResult(QTableWidget* const& table) {
    m_tableResult = table;
}

void MediatorGui::setTbResult(QWidget* const& tb) {
    m_tbResult = tb;
}

QString MediatorGui::getCmbText() const {
    return m_cmb->currentText();
}

int MediatorGui::getCmbCount() const {
    return m_cmb->count();
}

void MediatorGui::notifyCmbCom(int const& event, int const& idx) const {
    switch (event) {
    case KeyWords::start: {
        SerialCom serial;
        serial.fillCmb(m_cmb);
        m_cmb->setCurrentIndex(Settings::getInstance()->getCmbCom());
        break;
    }
    case KeyWords::indexChange: {
        if (idx == -1) { return; }
        QStringList list = m_cmb->itemData(idx).toStringList();
        m_lbDesc->setText(tr("Описание: %1").arg(list.count() > 1 ? list[1] : QString()));
        m_lbManuf->setText(tr("Производитель: %1").arg(list.count() > 2 ? list[2] : QString()));
        break;
    }
    case KeyWords::loadSettings:
        Settings::getInstance()->setCmbCom(m_cmb->currentIndex());
        break;
    default:
        break;
    }
}

void MediatorGui::notifyBtStBar(int const& event, QString const& text) const {
    switch (event) {
    case KeyWords::btEnable:
        m_btStart->setEnabled(1);
        m_btFlash->setEnabled(1);
        break;
    case KeyWords::btDisable:
        m_btStart->setEnabled(0);
        m_btFlash->setEnabled(0);
        break;
    case KeyWords::dataSent:
        m_stBar->showMessage("Данные отправлены");
        m_tabWidget->setCurrentWidget(m_tbResult);
        break;
    case KeyWords::noPorts:
        m_stBar->showMessage(tr("Нет доступных портов"));
        break;
    case KeyWords::portBusy:
        m_stBar->showMessage("Порт занят");
        break;
    case KeyWords::portOpen:
        m_stBar->showMessage(tr("Активный порт: %1").arg(text));
        m_btApply->setText(tr("Закрыть порт"));
        this->notifyBtStBar(KeyWords::btEnable);
        break;
    case KeyWords::portClose:
        m_stBar->showMessage(tr("Нет активного порта"));
        m_btApply->setText(tr("Открыть порт"));
        this->notifyBtStBar(KeyWords::btDisable);
        break;
    case KeyWords::loadFlash:
        m_stBar->showMessage("Записано " + text + " байт");
        break;
    case KeyWords::finishFlash:
        m_stBar->showMessage("Прошито");
        this->notifyBtStBar(KeyWords::btEnable);
        break;
    case KeyWords::failFlash:
        m_stBar->showMessage("Ошибка при записи");
        this->notifyBtStBar(KeyWords::btEnable);
        break;
    case KeyWords::wrongFormat:
        m_stBar->showMessage("Неправильный формат посылки, байт и данные: " + text);
        break;
    case KeyWords::timeout:
        m_stBar->showMessage("Таймаут");
        break;
    default:
        break;
    }
}

void MediatorGui::notifyTable(int const& event, const QString& name, const QString& result, const bool& check) {
    if (event == KeyWords::configTable) {
        this->configureTable(3);
    } else if (event == KeyWords::fillTable) {
        this->displayResultInTable(name, result, check);
    } else if (event == KeyWords::resetTable) {
        m_tableResult->clearContents();
        m_tableResult->setRowCount(0);
        m_rowIndex = 0;
    }
}

void MediatorGui::configureTable(size_t const& columnCount) {
    m_columnCount = columnCount;
    m_tableResult->clear();
    m_tableResult->setColumnCount(m_columnCount);
    m_tableResult->setShowGrid(true);
    m_tableResult->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableResult->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableResult->setHorizontalHeaderLabels({"Name", "Data", "Result"});
    m_tableResult->setColumnWidth(1, 200);
    m_tableResult->horizontalHeader()->setStretchLastSection(true);
    m_rowIndex = 0;
}

void MediatorGui::displayResultInTable(const QString &name, const QString &result, const bool &check) {
    m_tableResult->insertRow(m_rowIndex);
    QTableWidgetItem item[m_columnCount];
    item[0].setText(name);
    item[1].setText(result);
    if (check) {
        item[2].setText("OK");
        this->settingItemsInTable(item);
    } else {
        item[2].setText("FAILED");
        this->settingItemsInTable(item, QBrush(QColor(255, 0, 0)));
    }
    ++m_rowIndex;
}

void MediatorGui::settingItemsInTable(QTableWidgetItem* const& item, QBrush const&& brush) const {
    for (size_t i = 0; i < m_columnCount; ++i) {
        QTableWidgetItem* tempItem = new QTableWidgetItem(item[i]);
        tempItem->setFlags(tempItem->flags() & ~Qt::ItemIsEditable);
        tempItem->setForeground(brush);
        m_tableResult->setItem(m_rowIndex, i, tempItem);
        tempItem = nullptr;
    }
}
