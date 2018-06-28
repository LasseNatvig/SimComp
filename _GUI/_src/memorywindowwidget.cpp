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
#include <QString>
#include <QApplication>

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
    mainFrame = new QSplitter(this);

    /* LEFT */
    leftTabs = new QTabWidget(this);
        // Table
    memoryDisplay = new QTableWidget(this);
    memoryDisplay->setColumnCount(globals::MEMORYWINDOW_COLCOUNT);
    updateDisplayHeaders();
    memoryDisplay->setSelectionMode(QAbstractItemView::NoSelection);
    memoryDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memoryDisplay->resizeColumnsToContents();
    memoryDisplay->horizontalHeader()->setSectionResizeMode(
                QHeaderView::Stretch);
        // Memory map
    memoryMapContainer = new QWidget(this);
    memoryMap = new MemoryMap(this);

    memoryMapUpdateBtn = new QPushButton("Update", this);
    connect(memoryMapUpdateBtn, SIGNAL(clicked()),
            this, SLOT(updateMemoryMap()));
    memoryMapClearBtn = new QPushButton("Clear", this);
    connect(memoryMapClearBtn, SIGNAL(clicked()),
            this, SLOT(clearMemoryMap()));

            // Config box
    QHBoxLayout* setPixelSizeLayout = new QHBoxLayout;
    setPixelSizeLbl = new QLabel("<b>Pixel size: </b>");
    pixelSizeIn = new QLineEdit;
    pixelSizeIn->setText(QString::number(
                             globals::MEMORYWINDOW_PIXMAP_PIXSIZE));
    pixelInVal = new QIntValidator(0, 100);
    pixelSizeIn->setValidator(pixelInVal);
    setPixelSizeLayout->addWidget(setPixelSizeLbl);
    setPixelSizeLayout->addWidget(pixelSizeIn);

    QHBoxLayout* setWidthLayout = new QHBoxLayout;
    setWidthLbl = new QLabel("<b>Width: </b>");
    widthIn = new QLineEdit;
    widthIn->setText(QString::number(
                         globals::MEMORYWINDOW_PIXMAP_WIDTH));
    widthInVal = new QIntValidator(0, 500);
    widthIn->setValidator(widthInVal);
    setWidthLayout->addWidget(setWidthLbl);
    setWidthLayout->addWidget(widthIn);

    QHBoxLayout* setHeightLayout = new QHBoxLayout;
    setHeightLbl = new QLabel("<b>Height: </b>", this);
    heightIn = new QLineEdit(this);
    heightIn->setText(QString::number(
                          globals::MEMORYWINDOW_PIXMAP_HEIGHT));
    heightInVal = new QIntValidator(0, 500);
    heightIn->setValidator(heightInVal);
    setHeightLayout->addWidget(setHeightLbl);
    setHeightLayout->addWidget(heightIn);

    memoryMapBox = new QGroupBox;
    QVBoxLayout* memoryMapBoxLayout = new QVBoxLayout;
    memoryMapBoxLayout->addLayout(setPixelSizeLayout);
    memoryMapBoxLayout->addLayout(setWidthLayout);
    memoryMapBoxLayout->addLayout(setHeightLayout);
    memoryMapBoxLayout->addWidget(memoryMapUpdateBtn);
    memoryMapBoxLayout->addWidget(memoryMapClearBtn);
    memoryMapBoxLayout->setSpacing(1);
    memoryMapBox->setLayout(memoryMapBoxLayout);
    memoryMapBox->setFixedSize(memoryMapBox->sizeHint());

    toggleBtn = new QPushButton(
                QIcon(QApplication::style()->standardIcon(
                          QStyle::SP_TitleBarShadeButton)), "", this);
    connect(toggleBtn, SIGNAL(clicked()), this, SLOT(toggleMemoryMapMenu()));

    QVBoxLayout* memoryMapContainerLayout = new QVBoxLayout;
    memoryMapContainerLayout->addWidget(memoryMap);
    memoryMapContainerLayout->addWidget(memoryMapBox);
    memoryMapContainerLayout->addWidget(toggleBtn, 0, Qt::AlignBottom);
    memoryMapContainer->setLayout(memoryMapContainerLayout);

    leftTabs->addTab(memoryDisplay, "Table");
    leftTabs->addTab(memoryMapContainer, "Bitmap");


    /* RIGHT */
    rightContainer = new QWidget(this);
        // - TOP
    topDescriptionLbl = new QLabel("<b>Choose address area to display</b>", this);
    rightSpacer = new QSpacerItem(1,1);
    fromAddrLbl = new QLabel("From address: ", this);
    fromAddrSpnbox = new QSpinBox(this);
    fromAddrSpnbox->setMaximum(10000);
    toAddrLbl = new QLabel("To address: ",this);
    toAddrSpnbox = new QSpinBox(this);
    toAddrSpnbox->setMaximum(10000);

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
    midDescriptionLbl = new QLabel("<b>Choose memory type</b>", this);
    imBtn = new QRadioButton("Instruction Memory", this);
    dmBtn = new QRadioButton("Data Memory", this);

    midRightBox = new QGroupBox(this);
    QVBoxLayout* midRightLayout = new QVBoxLayout;
    midRightLayout->addWidget(midDescriptionLbl);
    imBtn->setChecked(true);
    midRightLayout->addWidget(imBtn);
    midRightLayout->addWidget(dmBtn);
    midRightBox->setLayout(midRightLayout);

        // - BOTTOM
    nameInput = new QLineEdit(this);
    setWindowNameLbl = new QLabel("<b>Name: </b>", this);
    updateBtn = new QPushButton("Update", this);
    clearBtn = new QPushButton("Clear", this);

    btmRightBox = new QGroupBox(this);
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
    mainFrame->addWidget(leftTabs);
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
    updateDisplays();
}

void MemoryWindowWidget::updateDisplays() {
    clearDisplay();
    std::vector<std::string> dump;
    dump = simulator->memoryDump(fromAddr, toAddr, memtyp);
    for (int i = 0; i < dump.size(); i++) {
        if (i % columnCount == 0)
            memoryDisplay->insertRow(memoryDisplay->rowCount());
        memoryDisplay->setItem(memoryDisplay->rowCount()-1, i % columnCount,
                               new QTableWidgetItem(QString::fromStdString(dump[i])));
    }
    if (memtyp == DATA)
        memoryMap->setVector(dump);
    updateDisplayHeaders();
}

void MemoryWindowWidget::updateMemoryMap() {
    memoryMap->setPixelSize(pixelSizeIn->text().toInt());
    memoryMap->setWidth(widthIn->text().toInt());
    memoryMap->setHeight(heightIn->text().toInt());
    memoryMap->update();
}

void MemoryWindowWidget::clearMemoryMap() {
    memoryMap->clear();
    memoryMap->update();
}

void MemoryWindowWidget::toggleMemoryMapMenu() {
    if (memoryMapBox->isVisible()) {
        memoryMapBox->hide();
        toggleBtn->setIcon(QIcon(QApplication::style()->standardIcon(
                                   QStyle::SP_TitleBarUnshadeButton)));
    } else {
        memoryMapBox->show();
        toggleBtn->setIcon(QIcon(QApplication::style()->standardIcon(
                                   QStyle::SP_TitleBarShadeButton)));
    }

}
