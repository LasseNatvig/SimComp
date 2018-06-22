#ifndef MEMORYWINDOWWIDGET_H
#define MEMORYWINDOWWIDGET_H

#include "../../_Simulator/_src/compSim.h"
#include "globals.h"
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include <QSpacerItem>
#include <QRadioButton>
#include <QString>
#include <QAction>

class MemoryWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryWindowWidget(QWidget *parent, ComputerSimulation* simulator);
private:
    ComputerSimulation* simulator;
    QLabel* title_lbl;
    QTableWidget* memoryDisplay;
    QAction* closeAction;
    int lastInstructionCount;
    int imTopRange;
    int dmTopRange;

    /*SIDE PANEL*/
    // -TOP
    QGroupBox* topSidepanel_box;
    QLabel* topDescription_lbl;
    QSpacerItem* sidepanel_spacer;
    QLabel* fromAddr_lbl;
    QSpinBox* fromAddr_spnbox;
    QLabel* toAddr_lbl;
    QSpinBox* toAddr_spnbox;
    // -MIDDLE
    QGroupBox* midSidepanel_box;
    QLabel* midDescription_lbl;
    QRadioButton* im_btn;
    QRadioButton* dm_btn;

    // -BOTTOM
    QGroupBox* btmSidepanel_box;
    QPushButton* update_btn;
    QPushButton* clear_btn;

signals:

public slots:

private slots:
    void clearDisplay();
    void update();
    void showErrorMessage(QString errorMsg);
};

#endif // MEMORYWINDOWWIDGET_H
