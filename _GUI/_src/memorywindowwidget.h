#ifndef MEMORYWINDOWWIDGET_H
#define MEMORYWINDOWWIDGET_H

#include "../../_Simulator/_src/compSim.h"
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include <QSpacerItem>
#include <QRadioButton>

class memoryWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit memoryWindowWidget(QWidget *parent, ComputerSimulation* simulator);
private:
    ComputerSimulation* simulator;
    QLabel* title_lbl;

    QTableWidget* memoryDisplay;

    /*SIDE PANEL*/
    // -TOP
    QGroupBox* topSidepanel_box;
    QLabel* topDescription_lbl;
    QSpacerItem* sidepanel_spacer;
    QLabel* fromAddr_lbl;
    QSpinBox* fromAddr_spnbox;
    QLabel* toAddr_lbl;
    QSpinBox* toAddr_spnbox;
    // MIDDLE
    QGroupBox* midSidepanel_box;
    QLabel* midDescription_lbl;
    QRadioButton* im_btn;
    QRadioButton* dm_btn;

    //-BOTTOM
    QGroupBox* btmSidepanel_box;
    QPushButton* update_btn;
    QPushButton* clear_btn;
    QPushButton* exit_btn;

signals:

public slots:

private slots:
    void clearDisplay();
    void update();
};

#endif // MEMORYWINDOWWIDGET_H
