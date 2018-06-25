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


RunWidget::RunWidget(QWidget *parent) : QWidget(parent)
{ 
    // Create instance of ComputerSimulation
    simulator = new ComputerSimulation(simName, "/Users/olebjorn/Desktop/GUI_logg.txt");

    // Create thread with simulator and make instance of timer
    simThread = new SimulatorThread(simulator);
    progressTimer = new QTimer;

    connect(simThread, SIGNAL(finished()), this, SLOT(runFinished()));
    connect(progressTimer, SIGNAL(timeout()), this, SLOT(updatePerformance()));

    createTabs();
    createMenuBar();
    createProgramInfo();
    createSidePanel();
    createWindows();

    // Group sidepanel in one layout
    QVBoxLayout* sidePanelLayout = new QVBoxLayout;
    sidePanelLayout->addWidget(icon_img, 0, Qt::AlignCenter); // TOP
    sidePanelLayout->addWidget(info_lst, 0, Qt::AlignCenter); // MIDDLE
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
    connect(reset_btn, SIGNAL(clicked()), this, SLOT(reset())); // reset_btn -> reset()
    connect(dropdownMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(setButtonText(int))); // Change in dropdown menu -> change start_btm

    // Set minimum size of this window
    this->setMinimumSize(QSize(MAINWINDOW_MIN_WIDTH, MAINWINDOW_MIN_HEIGHT));
}



/* ***** CORE SLOTS ***** */
void RunWidget::startSim() {
    /* startSim() starts the simulation with the selected mode from the dropdown menu */

    if (filename.isNull()) return; // Can not start simulation without filename

    // Decide which mode to start simulator in
    switch (dropdownMenu->currentIndex()) {
    case 0: // Run program
        run();
        break;
    case 1: // Single step program
        step();
        break;
    case 2: // Next breakpoint
        next();
        break;
    }
}


// ***** MODES *****
// STEP MODE
void RunWidget::step() {
    /* Moves to next step */
    if (simulationFinished) { // Simulation finshed
        writeInfo("Simulation already finished, press reset to start"
                           " a new simulation.");
        return;
    }
    if (simThread->isRunning()) {
        writeInfo("Wait until the simulator is done running..");
        return;
    }
    if (!simulator->isRunning()) load(); // Only load when simulator is not
                                         // running

    word currentPC = simulator->cpu->PC;
    addStep(currentPC);
    if ((!simulator->step())) // Make simulator execute one step
        simulationFinished = true;; // Simulation finshed

}

// RUN MODE
void RunWidget::run() {
    if (simulationFinished) { // Simulation finshed
        writeInfo("Simulation already finished, press reset to start"
                           " a new simulation.");
        return;
    }
    if (simThread->isRunning()) {
        writeInfo("Simulator is already running..");
        return;
    }
    runStop = false;
    load(); // Load program



    progressTimer->start(TIMER_UPDATE); // Start timer for updating progress
    runStart = clock();
    simThread->start();
}

void RunWidget::runFinished() {
    if (progressTimer->isActive())
        progressTimer->stop();
    if (runStop)
        writeInfo("Simulation aborted");
    else {
        addRunStats(runStart);
        simulationFinished = true;
    }
}

// NEXT MODE
void RunWidget::next() {
    if (simulationFinished) { // Simulation finshed
        writeInfo("Simulation already finished, press reset to start"
                           " a new simulation.");
        return;
    }
    if (simThread->isRunning()) {
        writeInfo("Wait until the simulator is done running..");
        return;
    }

    if (!simulator->isRunning()) load();
    std::vector<int> vev = ide->getBreakPoints();;
    simulator->setBreakPoints(vev);

    if (!simulator->next())
       simulationFinished = true;;

    addStep(simulator->cpu->PC);
}
// *****************


// **** UTILS ****
void RunWidget::load() {
    if (simThread->isRunning()) {
        writeInfo("Wait until simulator is done running..");
        return;
    }
    reset();
    simulator->load(filename.toStdString());
}

void RunWidget::openMemoryWindow() {
    if (simThread->isRunning()) {
        writeInfo("Wait until the simulator is done running..");
        return;
    }
    if (memoryWindow->isVisible())
        memoryWindow->activateWindow();
    else
        memoryWindow->show();
}

void RunWidget::openPreformanceWindow() {
    if (performanceChartView->isVisible())
        performanceChartView->activateWindow();
    else {
        performanceChartView->show();
    }
}

void RunWidget::reset() {
    simulationFinished = false;
    lastInstructionCount = 0; // Reset mips calculation for performance window
    performanceChart->reset(); // Resete performance chart
    progressTimer->stop(); // Stop timer
    if (simThread->isRunning()) {
        runStop = true;
        simThread->exit(); // Exit thread
    }
    simulator->reset(); // Reset simulator
    table->setRowCount(0); // Clear execution table
    info_lst->clear(); // Clear info list
}

void RunWidget::openFile() {
    // Starts a file dialog which lets the user choose a assembler program to run
    if (simThread->isRunning()) {
        writeInfo("Wait until the simulator is done running..");
        return;
    }
    const QString DEFAULT_DIR_KEY("\\");
    QSettings settings;

    QString selectedFile = QFileDialog::getOpenFileName(
                this, "Open", settings.value(DEFAULT_DIR_KEY).toString(), tr("Assembler program (*.sasm)"));

    if (!selectedFile.isEmpty()) {
        QDir currentDir;
        settings.setValue(DEFAULT_DIR_KEY,
                            currentDir.absoluteFilePath(selectedFile));
        filename = selectedFile;
        ide->open(filename);
        reset();
    }
}

void RunWidget::newFile() {
    ide->newFile();
}

void RunWidget::addRunStats(clock_t start) {
    /* Appends statistics from current simulation */
    std::stringstream  ss;

    writeInfo("**** RUN STATISTICS *****");
    ss << getMIPS(clock()-start);
    writeInfo(QString::fromStdString("MIPS: " + ss.str()));
    ss.str(std::string());
    ss << "Duration: " << static_cast<double>(clock()-start) / CLOCKS_PER_SEC
       << " sec.";
    writeInfo(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << "IM reads: " << simulator->IM.getMemoryRead();
    writeInfo(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << "IM writes: " << simulator->IM.getMemoryWrites();
    writeInfo(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << "DM reads: " << simulator->DM.getMemoryRead();
    writeInfo(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << "DM writes: " << simulator->DM.getMemoryWrites();
    writeInfo(QString::fromStdString(ss.str()));
    writeInfo("***********************");

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
    table->scrollToBottom();
}

void RunWidget::updateFilename(QString filename) {
    this->filename = filename;
    program_lbl->setText("<b>Program:</b> " + filename);
}

void RunWidget::runFromShortCut() {
    dropdownMenu->setCurrentIndex(0);
    startSim();
}

void RunWidget::stepFromShortCut() {
    dropdownMenu->setCurrentIndex(1);
    startSim();
}

void RunWidget::nextFromShortCut() {
    dropdownMenu->setCurrentIndex(2);
    startSim();
}

void RunWidget::updatePerformance() {
    int instructionCount = simulator->getInstructionsSimulated();
    double mips = (instructionCount - lastInstructionCount)/(TIMER_UPDATE*1000);
    lastInstructionCount = instructionCount;
    performanceChart->updatePerformance(mips);
}

double RunWidget::getMIPS(clock_t ticks) {
    double seconds = static_cast<double>(ticks) / CLOCKS_PER_SEC; // Calculate seconds
    return (static_cast<double>(simulator->getInstructionsSimulated()) / 1000000.0) / seconds; // Calculate MIPS
}

void RunWidget::setButtonText(int currentIndex) {
    switch (currentIndex) {
    case 0:
        start_btn->setText("Run");
        break;
    case 1:
        start_btn->setText("Step");
        break;
    case 2:
        start_btn->setText("Next");
    }
}

void RunWidget::writeInfo(QString info) {
    info_lst->addItem(info);
    info_lst->scrollToBottom();
}
// **********************



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
    stepAction = new QAction(tr("&Step"));
    nextAction = new QAction(tr("&Next"));
    resetAction = new QAction(tr("&Reset"));
    runAction->setShortcut(QKeySequence::Refresh);
    resetAction->setShortcut(QKeySequence(tr("Shift+r")));
    stepAction->setShortcut(QKeySequence(tr("Shift+s")));
    nextAction->setShortcut(QKeySequence(tr("Shift+n")));
    buildMenu->addAction(runAction);
    buildMenu->addAction(stepAction);
    buildMenu->addAction(nextAction);
    buildMenu->addSeparator();
    buildMenu->addAction(resetAction);
    connect(runAction, SIGNAL(triggered()), this, SLOT(runFromShortCut()));
    connect(stepAction, SIGNAL(triggered()), this, SLOT(stepFromShortCut()));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(nextFromShortCut()));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    viewMenu = menuBar->addMenu(tr("&View"));
    memoryAction = new QAction(tr("&Show Memory Window"));
    performanceAction = new QAction(tr("&Show Performance Window"));
    memoryAction->setShortcut(QKeySequence(tr("Ctrl+m")));
    performanceAction->setShortcut(QKeySequence(tr("Ctrl+p")));
    viewMenu->addAction(memoryAction);
    viewMenu->addAction(performanceAction);
    connect(memoryAction, SIGNAL(triggered()), this, SLOT(openMemoryWindow()));
    connect(performanceAction, SIGNAL(triggered()), this, SLOT(openPreformanceWindow()));
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
    dropdownMenu->addItem("Single step");
    dropdownMenu->addItem("Next breakpoint");
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
    info_lst = new QListWidget;
    info_lst->setSelectionMode(QAbstractItemView::NoSelection);
    info_lst->setWordWrap(true);

    // Create icon as QPixmap in a QLabel
    icon_img = new QLabel;
    QPixmap icon(":/images/../_img/SimComp_icon.png");
    icon_img->setPixmap(icon);

    // Create and group buttons
    buttonBox = new QGroupBox;
    start_btn = new QPushButton("Run");
    reset_btn = new QPushButton("Reset");

    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(start_btn);
    buttonsLayout->addWidget(reset_btn);
    buttonBox->setLayout(buttonsLayout);
}

void RunWidget::createTabs() {
    tabs = new QTabWidget(this);

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
    connect(ide, SIGNAL(filenameChange(QString)), this, SLOT(updateFilename(QString)));
}

void RunWidget::createWindows() {
    // Memory window
    memoryWindow = new MemoryWindowWidget(this, simulator);
    memoryWindow->setWindowFlag(Qt::Window);

    // Performance window
    performanceChart = new PerformanceChart;
    performanceChartView = new QChartView(performanceChart, this);
    closePerformanceAction = new QAction(tr("&Close"));
    closePerformanceAction->setShortcut(QKeySequence::Close);
    performanceChartView->addAction(closePerformanceAction);
    connect(closePerformanceAction, SIGNAL(triggered()), performanceChartView, SLOT(close()));
    performanceChartView->setWindowTitle("Performance");
    performanceChartView->setRenderHint(QPainter::Antialiasing);
    performanceChartView->setWindowFlag(Qt::Tool);
    performanceChartView->setFixedSize(QSize(PERFORMANCEWINDOW_MIN_WIDTH,
                                             PERFORMANCEWINDOW_MIN_HEIGHT));
}


/* THREAD */
SimulatorThread::SimulatorThread(ComputerSimulation* simulator) :
    simulator(simulator) { }

SimulatorThread::~SimulatorThread() {

}

void SimulatorThread::run() {
    simulator->run();
}



/* UTILS */
void showChange(QTableWidget* table, int rowIndex, int from, int to) {
    /* Changes color of cell that changed since previous row */
    if (table->rowCount() < 2) return;
    QTableWidgetItem* cell;
    for (int i = from; i < to; i++) {
        cell = table->item(rowIndex, i);
        if (!(table->item(rowIndex-1, i)->text() == cell->text()) &&
                !(cell->foreground().color() == QColor("red")))
            cell->setForeground(QBrush(QColor("red")));
        else
            cell->setForeground(QBrush(QColor("black")));
    }
}
