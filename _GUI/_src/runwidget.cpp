#include "runwidget.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPixmap>
#include <QSettings>
#include <QFileDialog>
#include <ctime>


RunWidget::RunWidget(QWidget *parent) : QWidget(parent)
{ 
    // Create instance of ComputerSimulation
    simulator = new ComputerSimulation(simName);

    createTabs();
    createMenuBar();
    createProgramInfo();
    createSidePanel();


    // Group sidepanel in one layout
    QVBoxLayout* sidePanelLayout = new QVBoxLayout;
    sidePanelLayout->addWidget(icon_img, 0, Qt::AlignCenter); // TOP
    sidePanelLayout->addWidget(stats_lst, 0, Qt::AlignCenter); // MIDDLE
    sidePanelLayout->addWidget(description_lbl, 0, Qt::AlignCenter); // MIDDLE
    sidePanelLayout->addWidget(buttonBox, 0, Qt::AlignBottom); // BOTTOM

    // Make main layout
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(programBox, 1, 1);
    mainLayout->addWidget(dropdownMenuBox, 1, 2);
    mainLayout->addWidget(tabs, 2, 1);
    mainLayout->addLayout(sidePanelLayout, 2, 2);

    // Set main layout of RunWidget
    setLayout(mainLayout);

    // Make connections
    connect(start_btn, SIGNAL(clicked()), this, SLOT(startSim())); // start_btn -> startSim()
    connect(reset_btn, SIGNAL(clicked()), this, SLOT(resetSim())); // reset_btn -> resetSim()
    connect(dump_btn, SIGNAL(clicked()), this, SLOT(memoryDump())); // dump_btn -> memoryDump()
    connect(dropdownMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(setButtonText(int))); // Change in dropdown menu -> change start_btm

    // Set minimum size of this window
    this->setMinimumSize(QSize(MIN_WIDTH, MIN_HEIGHT));
}


/* SLOTS */
void RunWidget::startSim() {
    /* startSim() starts the simulation with the selected mode from the dropdown menu */

    if (filename.isNull()) return; // Can not start simulation without filename

    // Decide which mode to start simulator in
    switch (dropdownMenu->currentIndex()) {
    case 0: // Run program
        run();
        break;
    case 1: // Singel step program
        step();
        break;
    }
}

void RunWidget::setButtonText(int currentIndex) {
    switch (currentIndex) {
    case 0:
        start_btn->setText("Run");
        break;
    case 1:
        start_btn->setText("Step");
        break;
    }
}

void RunWidget::step() {
    /* Moves to next step */
    if (simulationFinished) return; // Simulation finshed

    if (!simulator->isRunning()) load(); // Only load when simulator is not running

    word currentPC = simulator->cpu->PC;
    addStep(currentPC);
    if ((!simulator->step())) // Make simulator execute one step
        simulationFinished = true; // Simulation finshed

}

void RunWidget::run() {
    if (simulationFinished) return;
    load(); // Load program
    clock_t start = clock();
    simulator->run();
    simulationFinished = true;
    addStats(start);
}

void RunWidget::load() {
    resetSim();
    simulator->load(filename.toStdString());
}

void RunWidget::memoryDump() {
    memoryWindow = new MemoryWindowWidget(nullptr, simulator);
    memoryWindow->setAttribute(Qt::WA_DeleteOnClose); 
    memoryWindow->show();
}

void RunWidget::resetSim() {
    simulationFinished = false;
    table->setRowCount(0);
    stats_lst->clear();
}

void RunWidget::openFile() {
    // Starts a file dialog which lets the user choose a assembler program to run
    const QString DEFAULT_DIR_KEY("/");
    QSettings settings;

    QString selectedFile = QFileDialog::getOpenFileName(
                this, "Open", settings.value(DEFAULT_DIR_KEY).toString(), tr("Assembler program (*.sasm)"));

    if (!selectedFile.isEmpty()) {
        QDir currentDir;
        settings.setValue(DEFAULT_DIR_KEY,
                            currentDir.absoluteFilePath(selectedFile));
        filename = selectedFile;
        ide->open(filename);
        resetSim();
    }
}

void RunWidget::newFile() {

}

void RunWidget::addStats(clock_t start) {
    /* Appends statistics from current simulation */

    // Create vector with stats
    std::vector<std::string> dm_vec;
    std::vector<std::string> im_vec;
    std::stringstream  ss;
    ss << getMIPS(clock()-start);
    stats_lst->addItem(QString::fromStdString("MIPS: " + ss.str()));
    ss.str(std::string());
  //  ss << simulator->IM
}

void RunWidget::addStep(word PC) {
    // Add row for current step (TODO: -make better!)
    std::stringstream stepInfo;
    table->insertRow(table->rowCount());

    stepInfo << simulator->getInstructionsSimulated();
    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());

    stepInfo << simulator->memoryDump(PC, PC, INSTR)[0];
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());

    stepInfo << PC;
    table->setItem(table->rowCount()-1, 2, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());
    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++) {
        stepInfo << simulator->cpu->getRegister(i);
        table->setItem(table->rowCount()-1, i + 3, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
        stepInfo.str(std::string());
    }
    stepInfo << simulator->cpu->PC;
    table->setItem(table->rowCount()-1, 11, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));

    // Set color of registers that changed
    showChange(table,table->rowCount()-1, 3, simulator->cpu->getNumberOfRegisters()+3);
}

void RunWidget::updateProgramLabel(QString filename) {
    program_lbl->setText("<b>Program:</b> " + filename);
}

void RunWidget::runFromShortCut() {
    dropdownMenu->setCurrentIndex(0);
    startSim();
}

/* CREATE */
void RunWidget::createMenuBar() {
    menuBar = new QMenuBar;

    fileMenu = menuBar->addMenu(tr("&File"));
    newAction = new QAction(tr("&New"));
    openAction = new QAction(tr("&Open"));
    saveAction = new QAction(tr("&Save"));
    saveAsAction = new QAction(tr("&Save As"));
    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), ide, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), ide, SLOT(saveAs()));

    editMenu = menuBar->addMenu(tr("&Edit"));
    undoAction = new QAction(tr("&Undo"));
    redoAction = new QAction(tr("&Redo"));
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    connect(undoAction, SIGNAL(triggered()), ide, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), ide, SLOT(redo()));

    buildMenu = menuBar->addMenu(tr("&Build"));
    runAction = new QAction(tr("&Run"));
    runAction->setShortcut(QKeySequence::Refresh);
    buildMenu->addAction(runAction);
    connect(runAction, SIGNAL(triggered()), this, SLOT(runFromShortCut()));

}

void RunWidget::createProgramInfo() {
    program_lbl = new QLabel("<b>Computer Simulation</b><br>"
                             "Go to <i>File->Open/New</i> to open or create a new file.");
    programBox = new QGroupBox;
    QHBoxLayout* programLayout = new QHBoxLayout;
    programLayout->addWidget(program_lbl);
    programBox->setLayout(programLayout);
}

void RunWidget::createSidePanel() {
    // Create dropdown menu and group with label and add to
    dropdownMenuBox = new QGroupBox;
    select_mode_lbl = new QLabel("Select simulation mode");
    dropdownMenu = new QComboBox;
    select_mode_lbl->setStyleSheet("font-weight: bold;");
    QVBoxLayout* dropdownMenuLayout = new QVBoxLayout;
    dropdownMenu->addItem("Run");
    dropdownMenu->addItem("Singel step");
    dropdownMenuLayout->addWidget(select_mode_lbl, 0, Qt::AlignCenter);
    dropdownMenuLayout->addWidget(dropdownMenu);
    dropdownMenuBox->setLayout(dropdownMenuLayout);

    // Create and set text of description
    description_lbl = new QLabel;
    QString description_txt = "Simulator: AdHoc16_V03\n";
    description_txt.append("(Pressing \"Reset\" will stop the simulation and reset PC)");
    description_lbl->setText(description_txt);
    description_lbl->setStyleSheet("font-weight: bold;");

    // Create list with statistics
    stats_lst = new QListWidget;

    // Create icon as QPixmap in a QLabel
    icon_img = new QLabel;
    QPixmap icon(":/images/../_img/SimComp_icon.png");
    icon_img->setPixmap(icon);

    // Create and group buttons
    buttonBox = new QGroupBox;
    start_btn = new QPushButton("Run");
    dump_btn = new QPushButton("Memory Dump");
    reset_btn = new QPushButton("Reset");

    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(start_btn);
    buttonsLayout->addWidget(reset_btn);
    buttonsLayout->addWidget(dump_btn);
    buttonBox->setLayout(buttonsLayout);
}

void RunWidget::createTabs() {
    tabs = new QTabWidget;

    table = new QTableWidget;
    tableHeader << "Instruction count" << "OpCode" << "PC";
    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++)
        tableHeader << QString("R") + QString::number(i);
    tableHeader << "next PC";
    table->setColumnCount(12);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setHorizontalHeaderLabels(tableHeader);
    table->verticalHeader()->setVisible(false);

    ide = new IdeWidget(this);
    tabs->addTab(table, "Execution");
    tabs->addTab(ide, "Editor");
    connect(ide, SIGNAL(updateLabel(QString)), this, SLOT(updateProgramLabel(QString)));
}


/* UTILS */
double RunWidget::getMIPS(clock_t ticks) {
    double seconds = static_cast<double>(ticks) / CLOCKS_PER_SEC; // Calculate seconds
    return (static_cast<double>(simulator->getInstructionsSimulated()) / 1000000.0) / seconds; // Calculate MIPS
}

void showChange(QTableWidget* table, int rowIndex, int from, int to) {
    /* Changes color of cell that changed since previous row */
    if (table->rowCount() < 2) return;
    QTableWidgetItem* cell;
    for (int i = from; i < to; i++) {
        cell = table->item(rowIndex, i);
        if (!(table->item(rowIndex-1, i)->text() == cell->text()) && !(cell->foreground().color() == QColor("red")))
            cell->setForeground(QBrush(QColor("red")));
        else
            cell->setForeground(QBrush(QColor("black")));
    }
}
