#ifndef MEDIATORGUI_H
#define MEDIATORGUI_H

#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QStatusBar>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>

#include "settings.h"
#include "serialcom.h"

namespace KeyWords {
    enum Events {
        btDisable,
        btEnable,
        start,
        indexChange,
        loadSettings,
        dataSent,
        noPorts,
        portBusy,
        portOpen,
        portClose,
        loadFlash,
        finishFlash,
        failFlash,
        configTable,
        fillTable,
        resetTable,
        wrongFormat,
        timeout
    };
}

class MediatorGui : public QObject {
public:
    MediatorGui(QObject* parent = nullptr);

    void setLbManuf(QLabel* const& lb);
    void setLbDesc(QLabel* const& lb);
    void setBtStart(QPushButton* const& bt);
    void setBtApply(QPushButton* const& bt);
    void setBtFlash(QPushButton* const& bt);
    void setCmbCom(QComboBox* const& cmb);
    void setStBar(QStatusBar* const& stBar);
    void setTabWidget(QTabWidget* const& tabWidget);
    void setTableResult(QTableWidget* const& table);
    void setTbResult(QWidget* const& tb);
    QString getCmbText() const;
    int getCmbCount() const;
    void notifyCmbCom(int const& event, int const& idx = -1) const;
    void notifyBtStBar(int const& event, QString const& text = "") const;
    void notifyTable(int const& event, QString const& name = "", QString const& result = "", bool const& check = false);
    void configureTable(size_t const& columnCount);
    void displayResultInTable(QString const& name, QString const& result, bool const& check);
    void settingItemsInTable(QTableWidgetItem* const& item, QBrush const&& brush = QBrush(QColor(0, 0, 0))) const;

private:
    QWidget* m_tbResult;
    QTableWidget* m_tableResult;
    QTabWidget* m_tabWidget;
    QLabel* m_lbDesc;
    QLabel* m_lbManuf;
    QStatusBar* m_stBar;
    QComboBox* m_cmb;
    QPushButton* m_btStart;
    QPushButton* m_btApply;
    QPushButton* m_btFlash;
    size_t m_columnCount;
    size_t m_rowIndex;
};

#endif // MEDIATORGUI_H
