#include "memorywindowwidget.h"
#include <iostream>
#include <vector>
#include <string>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

MemoryWindowWidget::MemoryWindowWidget(QWidget *parent,
                                       ComputerSimulation* simulator) :
    QWidget(parent),
    simulator(simulator),
    lastInstructionCount(simulator->getInstructionsSimulated())
{
    /* Core */
    title_lbl = new QLabel("Memory display");
    closeAction = new QAction(tr("&Close"));
    closeAction->setShortcut(QKeySequence::Close);
    this->addAction(closeAction);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    // Table
    memoryDisplay = new QTableWidget;
    memoryDisplay->setColumnCount(10);
    memoryDisplay->setSelectionMode(QAbstractItemView::NoSelection);
    memoryDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    connect(fromAddr_spnbox, SIGNAL(editingFinished()), this,SLOT(update()));

    /* Configure window */
    this->setLayout(mainLayout);
    this->setWindowTitle("Memory Window");
    this->setMinimumSize(
                QSize(globals::MEMORYWINDOW_MIN_WIDTH, globals::MEMORYWINDOW_MIN_HEIGHT));
}

void MemoryWindowWidget::clearDisplay() {
    memoryDisplay->setRowCount(0);
}

void MemoryWindowWidget::update() {
    if (simulator->getMode() == RUNNING) return;
    int fromAddr = fromAddr_spnbox->value();
    int toAddr = toAddr_spnbox->value();

    if (fromAddr > toAddr) return;
    clearDisplay();
    std::vector<std::string> dump;
    memType memtyp = INSTR;
    unsigned int columnCount = 1;
    if (dm_btn->isChecked()) {
        memtyp = DATA;
        columnCount = 10;
    }
    memoryDisplay->setColumnCount(columnCount);
    dump = simulator->memoryDump(fromAddr, toAddr, memtyp);
    for (int i = 0; i < dump.size(); i++) {
        if (i % columnCount == 0)
            memoryDisplay->insertRow(memoryDisplay->rowCount());
        memoryDisplay->setItem(memoryDisplay->rowCount()-1, i % columnCount,
                               new QTableWidgetItem(QString::fromStdString(dump[i])));
    }
    memoryDisplay->resizeColumnsToContents();
}

void MemoryWindowWidget::showErrorMessage(QString errorMsg) {

}
