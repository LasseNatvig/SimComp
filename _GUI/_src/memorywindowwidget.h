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
#include <QSplitter>
#include <QLineEdit>
#include <QStringList>

class MemoryWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryWindowWidget(QWidget *parent, ComputerSimulation* simulator);
private:
    ComputerSimulation* simulator;
    memType memtyp = INSTR;
    int lastInstructionCount;
    int fromAddr = 0;
    int toAddr = 0;
    int columnCount = 1;

    /* MAIN FRAME */
    QSplitter* mainFrame;

    // LEFT SIDE
    QTableWidget* memoryDisplay;
    QStringList topDisplayHeader;
    QStringList sideDisplayHeader;

    // RIGHT SIDE
    QWidget* rightContainer;
        // -TOP
    QGroupBox* topRightBox;
    QLabel* topDescriptionLbl;
    QSpacerItem* rightSpacer;
    QLabel* fromAddrLbl;
    QSpinBox* fromAddrSpnbox;
    QLabel* toAddrLbl;
    QSpinBox* toAddrSpnbox;
        // -MIDDLE
    QGroupBox* midRightBox;
    QLabel* midDescriptionLbl;
    QRadioButton* imBtn;
    QRadioButton* dmBtn;
        // -BOTTOM
    QGroupBox* btmRightBox;
    QLineEdit* nameInput;
    QLabel* setWindowNameLbl;
    QPushButton* updateBtn;
    QPushButton* clearBtn;
    void createMainFrame();
signals:
    void windowNameChanged(QString name);

public slots:

private slots:
    void clearDisplay();
    void updateDisplayHeaders();
    void updateConfig();
    void updateDisplay();
};

#endif // MEMORYWINDOWWIDGET_H
