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
#include <QApplication>


RunWidget::RunWidget(QWidget *parent) : QWidget(parent)
{ 
    // Create instance of ComputerSimulation
    simulator = new ComputerSimulation(simName, "/Users/olebjorn/Desktop/GUI_logg.txt");

    // Create thread with simulator and make instance of timer
    simThread = new SimulatorThread(simulator);
    progressTimer = new QTimer;

    connect(simThread, SIGNAL(finished()), this, SLOT(runFinished()));
    connect(progressTimer, SIGNAL(timeout()), this, SLOT(updatePerformance()));

    // Create widgets
    createTabs();

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(tabs, 0, 1);
    setLayout(mainLayout);

    // Set minimum size of this window
    this->setMinimumSize(QSize(globals::MAINWINDOW_MIN_WIDTH, globals::MAINWINDOW_MIN_HEIGHT));
}



/* MODES */
void RunWidget::step() {
    /* Moves to next step */
    if (!validStart())
        return;
    if (!simulator->isRunning()) load(); // Only load when simulator is not
                                         // running
    word currentPC = simulator->cpu->PC;
    addStep(currentPC);
    emit memoryChanged();
    if (!simulator->step()) // Make simulator execute one step
        simulationFinished = true;; // Simulation finshed

    // Add next PC to table
    addNextPC();

    tabs->setCurrentIndex(0);
    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
}

void RunWidget::run() {
    if (!validStart())
        return;
    runStop = false;
    load(); // Load program

    progressTimer->start(globals::TIMER_UPDATE); // Start timer for updating progress
    runStart = clock();
    simThread->start();
}

void RunWidget::runFinished() {
    if (progressTimer->isActive())
        progressTimer->stop();
    if (runStop)
        emit output("Simulation stopped.");
    else {
        simulationFinished = true;
        emit memoryChanged();
    }
}

void RunWidget::next() {
    if (!validStart())
        return;

    if (!simulator->isRunning()) load();
    std::vector<int> vev = ide->getBreakPoints();;
    simulator->setBreakPoints(vev);

    if (!simulator->next())
       simulationFinished = true;
    addStep(simulator->cpu->PC);
    addNextPC();

    tabs->setCurrentIndex(0);
    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
    emit memoryChanged();
}



/* EDITOR UTILS */
void RunWidget::load() {
    if (simThread->isRunning()) {
        emit output("Wait until simulator is done running..");
        return;
    }
    reset();
    simulator->load(filename.toStdString());
}

void RunWidget::reset() {
    simulationFinished = false;
    lastInstructionCount = 0; // Reset mips calculation for performance window
    progressTimer->stop(); // Stop timer
    if (simThread->isRunning()) {
        runStop = true;
        simThread->exit(); // Exit thread
    }
    simulator->reset(); // Reset simulator
    executionTable->setRowCount(0); // Clear execution table
    emit memoryChanged();
}

void RunWidget::openFile() {
    // Starts a file dialog which lets the user choose a assembler program to run
    if (simThread->isRunning()) {
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
        tabs->setCurrentIndex(1);
    }
}

void RunWidget::newFile() {
    ide->newFile();
}

void RunWidget::save() {
    ide->save();
}

void RunWidget::saveAs() {
    ide->saveAs();
}

void RunWidget::undo() {
    ide->undo();
}

void RunWidget::redo() {
    ide->redo();
}

void RunWidget::updateFilename(QString filename) {
    this->filename = filename;
}



/* EXECUTION UTILS */
void RunWidget::addStep(word PC) {
    // Add row for current step (TODO: -make better!)
    std::stringstream stepInfo;
    executionTable->insertRow(executionTable->rowCount());

    stepInfo << PC;
    executionTable->setItem(executionTable->rowCount()-1, 0,
                            new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());

    stepInfo << simulator->memoryDump(PC, PC, INSTR)[0];
    executionTable->setItem(executionTable->rowCount()-1, 1,
                            new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());
    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++) {
        stepInfo << simulator->cpu->getRegister(i);
        executionTable->setItem(executionTable->rowCount()-1, i + 2,
                                new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
        stepInfo.str(std::string());
    }

    // Set color of registers that changed
    showChange(executionTable, executionTable->rowCount()-1, 2,
               simulator->cpu->getNumberOfRegisters()+2);
    executionTable->scrollToBottom();
    executionTable->resizeColumnToContents(1);
}

void RunWidget::addNextPC() {
    std::stringstream stepInfo;
    stepInfo << simulator->cpu->PC;
    executionTable->setItem(executionTable->rowCount()-1, 10, new QTableWidgetItem(QString::fromStdString(stepInfo.str())));
}

void RunWidget::updatePerformance() {
    int instructionCount = simulator->getInstructionsSimulated();
    double mips = (instructionCount - lastInstructionCount)/(globals::TIMER_UPDATE*1000);
    lastInstructionCount = instructionCount;
    emit performanceChanged(mips);
    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
}

bool RunWidget::validStart() {
    if (filename.isNull()) {
        emit output("No program selected");
        return false;
    }
    if (simulationFinished) {
        emit output("Simulation already finished, press reset to start"
                    " a new simulation.");
        return false;
    }
    if (simThread->isRunning()) {
        emit output("Wait until simulation is done running...");
        return false;
    }
    return true;
}

double RunWidget::getMIPS(clock_t ticks) {
    double seconds = static_cast<double>(ticks) / CLOCKS_PER_SEC; // Calculate seconds
    return (static_cast<double>(simulator->getInstructionsSimulated()) / 1000000.0) / seconds; // Calculate MIPS
}



/* CREATE */
void RunWidget::createTabs() {
    tabs = new QTabWidget(this);

    executionTable = new QTableWidget;
    tableHeader << "PC" << "OpCode";
    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++)
        tableHeader << QString("R") + QString::number(i);
    tableHeader << "next PC";
    executionTable->setColumnCount(11);
    executionTable->setSelectionMode(QAbstractItemView::NoSelection);
    executionTable->setHorizontalHeaderLabels(tableHeader);
    executionTable->verticalHeader()->setVisible(false);
    executionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < 11; i++) {
        if (!(i == 1))
            executionTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    tabs->addTab(executionTable, "Execution");

    ide = new IdeWidget(this);
    tabs->addTab(ide, "Editor");
    connect(ide, SIGNAL(filenameChanged(QString)), this, SLOT(updateFilename(QString)));
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
ComputerSimulation* RunWidget::getSimulator() {
    return simulator;
}

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
