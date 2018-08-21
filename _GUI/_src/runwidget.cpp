#include "runwidget.h"
#include "../../_Simulator/_src/compSim.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <QGridLayout>
#include <QHeaderView>
#include <QSettings>
#include <QFileDialog>
#include <QTableWidget>
#include <QTimer>
#include <QDebug>


RunWidget::RunWidget(QWidget *parent) : QWidget(parent) {
    // Create instance of ComputerSimulation
    simulator = new ComputerSimulation(simName,
                                       "/Users/olebjorn/Desktop/GUI_logg.txt");

    // Create thread with simulator and create instance of timer
    simThread = new SimulatorThread(simulator);
    progressTimer = new QTimer;

    // Make connections
    connect(simThread, &SimulatorThread::finished, this, &RunWidget::runFinished);
    connect(progressTimer, &QTimer::timeout, this, &RunWidget::updatePerformance);

    // Create table
    createTable();

    // Create and populate layout
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(executionTable, 0, 1);
    setLayout(mainLayout);

    // Set minimum size of this widget
    this->setMinimumSize(QSize(globals::MAINWINDOW_MIN_WIDTH,
                               globals::MAINWINDOW_MIN_HEIGHT));
}

RunWidget::~RunWidget() {
    delete progressTimer;
    progressTimer = nullptr;
}

/* Simulator */
void RunWidget::run() {
    if (!validStart())
        return;
    runStop = false;
    progressTimer->start(globals::TIMER_UPDATE); // Start timer for
                                                 // updating progress
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
        double mips = getMIPS(clock()-runStart);
        emit output("Simulation finished. MIPS: " + QString::number(mips));
        emit memoryChanged();
        emit updatePerformance();
    }
}

void RunWidget::step() {
    if (!validStart())
        return;

    addStep(simulator->cpu->PC); // Show step and memory from the
    emit memoryChanged();        // instruction before the one that is excuted
    if (!simulator->step()) // Make simulator execute one step
        simulationFinished = true; // Simulation finshed

    // Add next PC to table
    addNextPC();

    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
}

void RunWidget::next() {
    if (!validStart())
        return;

    simulator->setBreakpoints(breakpoints);
    if (!simulator->next()) // Run until, but not including,
       simulationFinished = true; // the next instruction with breakpoint
    addStep(simulator->cpu->PC);
    addNextPC();

    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
    emit memoryChanged();
}

void RunWidget::load(QString filename) {
    this->filename = filename;
    if (simThread->isRunning()) {
        emit output("Wait until simulator is done running..");
        return;
    }
    QFile test(filename);
    if (!test.open(QFile::ReadOnly | QFile::Text))
        emit output("Could not open " + filename);
    reset();
    if (!test.size())
        return;
    else
        simulator->load(filename.toStdString());
}

void RunWidget::reset() {
    simulationFinished = false;
    lastInstructionCount = 0; // Reset mips calculation
    progressTimer->stop(); // Stop timer
    if (simThread->isRunning()) {
        runStop = true;
        simThread->exit(); // Exit thread
    }
    simulator->reset(); // Reset simulator
    executionTable->setRowCount(0); // Clear execution table
    emit memoryChanged();
}

void RunWidget::setBreakpoints(std::vector<int> breakpoints) {
    this->breakpoints = breakpoints;
}



/* Create */
void RunWidget::createTable() {
    executionTable = new QTableWidget;
    tableHeader << "PC" << "Instruction";
    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++)
        tableHeader << QString("R") + QString::number(i);
    tableHeader << "Next PC";
    executionTable->setColumnCount(11);
    executionTable->setSelectionMode(QAbstractItemView::NoSelection);
    executionTable->setHorizontalHeaderLabels(tableHeader);
    executionTable->verticalHeader()->setVisible(false);
    executionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < 11; i++)
        if (!(i == 1))
            executionTable->horizontalHeader()->
                    setSectionResizeMode(i, QHeaderView::Stretch);

}



/* Thread */
SimulatorThread::SimulatorThread(ComputerSimulation* simulator) :
    simulator(simulator) { }

SimulatorThread::~SimulatorThread() {

}

void SimulatorThread::run() {
    simulator->run();
}



/* Utils */
void RunWidget::addStep(word PC) {
    // Add row for current step (TODO: -make better!)
    std::stringstream stepInfo;
    executionTable->insertRow(executionTable->rowCount());

    stepInfo << PC;
    executionTable->setItem(executionTable->rowCount()-1, 0,
                            new QTableWidgetItem(
                                QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());

    stepInfo << simulator->memoryDump(PC, PC, INSTR)[0];
    executionTable->setItem(executionTable->rowCount()-1, 1,
                            new QTableWidgetItem(
                                QString::fromStdString(stepInfo.str())));
    stepInfo.str(std::string());

    for (int i = 0; i < simulator->cpu->getNumberOfRegisters(); i++) {
        stepInfo << simulator->cpu->getRegister(i);
        executionTable->setItem(executionTable->rowCount()-1, i + 2,
                                new QTableWidgetItem(
                                    QString::fromStdString(stepInfo.str())));
        stepInfo.str(std::string());
    }

    // Set color of registers that changed
    QPalette palette = executionTable->palette();
    QBrush brush = palette.brush(QPalette::Foreground);
    RunWidget::showChange(executionTable, brush, executionTable->rowCount()-1,
                          2, simulator->cpu->getNumberOfRegisters()+2);
    executionTable->scrollToBottom();
    executionTable->resizeColumnToContents(1);
}

void RunWidget::addNextPC() {
    std::stringstream stepInfo;
    stepInfo << simulator->cpu->PC;
    executionTable->setItem(executionTable->rowCount()-1, 10,
                            new QTableWidgetItem(
                                QString::fromStdString(stepInfo.str())));
}

void RunWidget::updatePerformance() {
    int instructionCount = simulator->getInstructionsSimulated();
    double mips = (instructionCount - lastInstructionCount)/
            (globals::TIMER_UPDATE*1000);
    lastInstructionCount = instructionCount;
    emit performanceChanged(mips);
    emit instructionCountChanged(
                simulator->getInstructionsSimulated());
}

bool RunWidget::validStart() {
    if (!simulator->validProgram()) {
        emit output("No program selected.");
        return false;
    }
    if (simulationFinished) {
        emit output("Simulation already finished, press reset to start"
                    " a new simulation.");
        return false;
    }
    if (simThread->isRunning()) {
        emit output("Wait until simulation is done...");
        return false;
    }
    return true;
}

double RunWidget::getMIPS(clock_t ticks) {
    double seconds = static_cast<double>(ticks) / CLOCKS_PER_SEC;
    return (static_cast<double>(simulator->getInstructionsSimulated()) /
            1000000.0) / seconds;
}

ComputerSimulation* RunWidget::getSimulator() {
    return simulator;
}

void RunWidget::showChange(QTableWidget* table, const QBrush &defaultBrush,
                           int rowIndex, int from, int to) {
    // Changes color of cell that changed since previous row
    if (table->rowCount() < 2) return;
    QTableWidgetItem* cell;
    for (int i = from; i < to; i++) {
        cell = table->item(rowIndex, i);
        if (!(table->item(rowIndex-1, i)->text() == cell->text()) &&
                !(cell->foreground().color() == QColor("red")))
            cell->setForeground(QBrush(QColor("red")));
        else
            cell->setForeground(defaultBrush);
    }
}
