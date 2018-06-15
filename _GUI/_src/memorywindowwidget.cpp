#include "memorywindowwidget.h"
#include <iostream>
#include <vector>
#include <string>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

memoryWindowWidget::memoryWindowWidget(QWidget *parent, ComputerSimulation* simulator) : QWidget(parent),
    simulator(simulator)
{
    /* Allocate objects */
    title_lbl = new QLabel("Memory display");

    // Table
    memoryDisplay = new QTableWidget;
    memoryDisplay->setColumnCount(10);
    memoryDisplay->setSelectionMode(QAbstractItemView::NoSelection);

    // Side panel
    topSidepanel_box = new QGroupBox; // - TOP
    topDescription_lbl = new QLabel("Choose address area to display");
    sidepanel_spacer = new QSpacerItem(1,1);
    fromAddr_lbl = new QLabel("From address: ");
    fromAddr_spnbox = new QSpinBox;
    toAddr_lbl = new QLabel("To address: ");
    toAddr_spnbox = new QSpinBox;

    midSidepanel_box = new QGroupBox; // - MIDDLE
    midDescription_lbl = new QLabel("Choose memory type");
    im_btn = new QRadioButton("Instruction Memory");
    dm_btn = new QRadioButton("Data Memory");

    btmSidepanel_box = new QGroupBox; // - BOTTOM
    update_btn = new QPushButton("Update");
    clear_btn = new QPushButton("Clear");
    exit_btn = new QPushButton("Exit");

    /* Add styling */
    title_lbl->setStyleSheet("font-weight: bold");
    topDescription_lbl->setStyleSheet("font-weight: bold;");
    midDescription_lbl->setStyleSheet("font-weight: bold");

    /* Group widgets */
    // Side panel
    QVBoxLayout* topSidepanel_layout = new QVBoxLayout; // - TOP
    topSidepanel_layout->addWidget(topDescription_lbl);
    topSidepanel_layout->addSpacerItem(sidepanel_spacer);
    topSidepanel_layout->addWidget(fromAddr_lbl);
    topSidepanel_layout->addWidget(fromAddr_spnbox);
    topSidepanel_layout->addWidget(toAddr_lbl);
    topSidepanel_layout->addWidget(toAddr_spnbox);
    topSidepanel_box->setLayout(topSidepanel_layout);

    QVBoxLayout* midSidepanel_layout = new QVBoxLayout; // - MIDDLE
    midSidepanel_layout->addWidget(midDescription_lbl);
    im_btn->setChecked(true);
    midSidepanel_layout->addWidget(im_btn);
    midSidepanel_layout->addWidget(dm_btn);
    midSidepanel_box->setLayout(midSidepanel_layout);

    QVBoxLayout* btmSidepanel_layout = new QVBoxLayout; // - BOTTOM
    btmSidepanel_layout->addWidget(update_btn);
    btmSidepanel_layout->addWidget(clear_btn);
    btmSidepanel_layout->addWidget(exit_btn);
    btmSidepanel_box->setLayout(btmSidepanel_layout);

    QVBoxLayout* sidepanel_layout = new QVBoxLayout;
    sidepanel_layout->addWidget(topSidepanel_box, 0, Qt::AlignTop);
    sidepanel_layout->addWidget(midSidepanel_box, 0, Qt::AlignCenter);
    sidepanel_layout->addWidget(btmSidepanel_box, 0, Qt::AlignBottom);

    /* Add widgets to main layout */
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(title_lbl, 0, 0, Qt::AlignCenter);
    mainLayout->addWidget(memoryDisplay, 1, 0);
    mainLayout->addLayout(sidepanel_layout, 1, 1);

    /* Make connections */
    connect(clear_btn, SIGNAL(clicked()), this, SLOT(clearDisplay()));
    connect(update_btn, SIGNAL(clicked()), this, SLOT(update()));
    connect(exit_btn, SIGNAL(clicked(bool)), this, SLOT(close()));

    /* Configure window */
    this->setLayout(mainLayout);
    this->setWindowTitle("Memory Dump");
}

void memoryWindowWidget::clearDisplay() {
    memoryDisplay->setRowCount(0);
}

void memoryWindowWidget::update() {
    int fromAddr = fromAddr_spnbox->value();
    int toAddr = toAddr_spnbox->value();

    if (fromAddr >= toAddr) return;
    clearDisplay();
    std::vector<std::string> dump;
    memType memtyp = INSTR;
    if (dm_btn->isChecked())
        memtyp = DATA;
    dump = simulator->memoryDump(fromAddr, toAddr, memtyp);
    if (dump.empty()) return;

    for (int i = 0; i < dump.size(); i++) {
        if (i % 10 == 0)
            memoryDisplay->insertRow(memoryDisplay->rowCount());
        memoryDisplay->setItem(memoryDisplay->rowCount()-1, i % 10, new QTableWidgetItem(QString::fromStdString(dump[i])));
    }
}

