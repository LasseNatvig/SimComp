#include "memorywindowwidget.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

MemoryWindowWidget::MemoryWindowWidget(QWidget *parent,
                                       ComputerSimulation* simulator) :
    QWidget(parent),
    simulator(simulator),
    lastInstructionCount(simulator->getInstructionsSimulated()) {

    createMainFrame();

    /* Add main frame to main layout */
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(mainFrame, 1, 0);

    /* Configure window */
    this->setLayout(mainLayout);
    this->setMinimumSize(
                QSize(globals::MEMORYWINDOW_MIN_WIDTH,
                      globals::MEMORYWINDOW_MIN_HEIGHT));
}



/* CREATE */
void MemoryWindowWidget::createMainFrame() {
    mainFrame = new QSplitter;

    // LEFT
    memoryDisplay = new QTableWidget(this);
    memoryDisplay->setColumnCount(globals::MEMORYWINDOW_COLCOUNT);
    updateDisplayHeaders();
    memoryDisplay->setSelectionMode(QAbstractItemView::NoSelection);
    memoryDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memoryDisplay->resizeColumnsToContents();
    memoryDisplay->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // RIGHT
    rightContainer = new QWidget(this);

        // - TOP
    topDescriptionLbl = new QLabel("<b>Choose address area to display</b>");
    rightSpacer = new QSpacerItem(1,1);
    fromAddrLbl = new QLabel("From address: ");
    fromAddrSpnbox = new QSpinBox;
    toAddrLbl = new QLabel("To address: ");
    toAddrSpnbox = new QSpinBox;

    topRightBox = new QGroupBox;
    QVBoxLayout* topRightLayout = new QVBoxLayout; // - TOP
    topRightLayout->addWidget(topDescriptionLbl);
    topRightLayout->addSpacerItem(rightSpacer);
    topRightLayout->addWidget(fromAddrLbl);
    topRightLayout->addWidget(fromAddrSpnbox);
    topRightLayout->addWidget(toAddrLbl);
    topRightLayout->addWidget(toAddrSpnbox);
    topRightBox->setLayout(topRightLayout);

        // - MIDDLE
    midDescriptionLbl = new QLabel("<b>Choose memory type</b>");
    imBtn = new QRadioButton("Instruction Memory");
    dmBtn = new QRadioButton("Data Memory");

    midRightBox = new QGroupBox;
    QVBoxLayout* midRightLayout = new QVBoxLayout;
    midRightLayout->addWidget(midDescriptionLbl);
    imBtn->setChecked(true);
    midRightLayout->addWidget(imBtn);
    midRightLayout->addWidget(dmBtn);
    midRightBox->setLayout(midRightLayout);

        // - BOTTOM
    nameInput = new QLineEdit;
    setWindowNameLbl = new QLabel("<b>Name: </b>");
    updateBtn = new QPushButton("Update");
    clearBtn = new QPushButton("Clear");

    btmRightBox = new QGroupBox;
    QVBoxLayout* btmRightLayout = new QVBoxLayout; // - BOTTOM
    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(setWindowNameLbl);
    inputLayout->addWidget(nameInput);
    btmRightLayout->addLayout(inputLayout);
    btmRightLayout->addWidget(updateBtn);
    btmRightLayout->addWidget(clearBtn);
    btmRightBox->setLayout(btmRightLayout);

    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearDisplay()));
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateConfig()));

    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addWidget(topRightBox, 0, Qt::AlignTop);
    rightLayout->addWidget(midRightBox, 0, Qt::AlignCenter);
    rightLayout->addWidget(btmRightBox, 0, Qt::AlignBottom);
    rightContainer->setLayout(rightLayout);

    // Add LEFT and RIGHT to main frame
    mainFrame->addWidget(memoryDisplay);
    mainFrame->addWidget(rightContainer);
    mainFrame->setSizes(QList<int>({0,1}));
}



/* UTILS */
void MemoryWindowWidget::clearDisplay() {
    memoryDisplay->setRowCount(0);
}

void MemoryWindowWidget::updateDisplayHeaders() {
    std::stringstream ss;
    topDisplayHeader.clear();
    sideDisplayHeader.clear();

    for (int i = fromAddr; i < fromAddr + memoryDisplay->columnCount(); i++) {
        ss << std::hex << i;
        topDisplayHeader << QString::fromStdString(ss.str());
        ss.str(std::string());
    }
    for (int i = 1; i < memoryDisplay->rowCount()+1; i++) {
        ss << std::hex << i;
        sideDisplayHeader << QString::fromStdString(ss.str());
        ss.str(std::string());
    }
    memoryDisplay->setHorizontalHeaderLabels(topDisplayHeader);
    memoryDisplay->setVerticalHeaderLabels(sideDisplayHeader);
}

void MemoryWindowWidget::updateConfig() {
    if (dmBtn->isChecked()) {
        memtyp = DATA;
        columnCount = globals::MEMORYWINDOW_COLCOUNT;
    } else {
        memtyp = INSTR;
        columnCount = 1;
    }
    memoryDisplay->setColumnCount(columnCount);
    fromAddr = fromAddrSpnbox->value();
    toAddr = toAddrSpnbox->value();
    if (fromAddr > toAddr) {
        fromAddr = toAddr = 0;
        return;
    }
    emit windowNameChanged(nameInput->displayText());
    updateDisplay();
}

void MemoryWindowWidget::updateDisplay() {
    clearDisplay();
    std::vector<std::string> dump;
    dump = simulator->memoryDump(fromAddr, toAddr, memtyp);
    for (int i = 0; i < dump.size(); i++) {
        if (i % columnCount == 0)
            memoryDisplay->insertRow(memoryDisplay->rowCount());
        memoryDisplay->setItem(memoryDisplay->rowCount()-1, i % columnCount,
                               new QTableWidgetItem(QString::fromStdString(dump[i])));
    }
    updateDisplayHeaders();
}

