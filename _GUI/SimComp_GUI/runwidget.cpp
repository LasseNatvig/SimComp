#include "runwidget.h"

#include <sstream>
#include <iomanip>
#include <QGridLayout>
#include <ctime>
#include <iostream>

runWidget::runWidget(QWidget *parent, QString filename) : filename(filename), QWidget(parent)
{
    // Create labels and buttons
    description_lbl = new QLabel("Select simulation mode.");
    start_btn = new QPushButton("Start");

    // Create dropdown menu and add options
    dropdownMenu = new QComboBox;
    dropdownMenu->addItem("Run");
    dropdownMenu->addItem("Singel step");
    dropdownMenu->addItem("Run and dump statistics");

    // Create layouts
    QGridLayout* gridLayout = new QGridLayout;

    // Add widgets to layout
    gridLayout->addWidget(description_lbl, 0, 0);
    gridLayout->addWidget(dropdownMenu, 1, 0);
    gridLayout->addWidget(start_btn, 1,1);

    // Set layout for the window(widget)
    this->setLayout(gridLayout);

    // Make connections
    connect(start_btn, SIGNAL(clicked()), this, SLOT(startSim())); // When start_btn is pressed call function startSim()
}

void runWidget::startSim() {
    /* startSim() starts the simulation with the selected mode from the dropdown menu */

    // Create instance of ComputerSimulation
    simulator = new ComputerSimulation(simName);

    // Load .sasm file
    simulator->load(filename.toStdString());

    clock_t start = readTime(); // Start timer (Should be changed to use QTimer)
    switch (dropdownMenu->currentIndex()) { // Decide which mode to start simulator in
        case 0: // Run program
            simulator->setRunning(true);
            simulator->runProgram(); // Run program
            break;
        case 1: // Singel step program
            simulator->setSingleStepModeGUI(true);
            simulator->setRunning(true);
            this->startSingleStepMode();
            break;
        case 2: // Run and dump statistics
            simulator->setDumpMode(true);
            simulator->setRunning(true);
            simulator->runProgram(); // Run program
            break;
    }
    double mips = this->getMIPS(readTime()-start);
    if (simulator->dump()) simulator->resetStatistics(simulator->cpu); // From core code (Not sure why this is done here)


    if (!simulator->singleStepGUI()) std::cout << mips; // Present MIPS nicely
    if (simulator->dump()) std::cout <<"Stats"; // Show statictics in a nice way
}

double runWidget::getMIPS(clock_t ticks) {
    double seconds = static_cast<double>(ticks) / CLOCKS_PER_SEC; // Calculate seconds
    return (static_cast<double>(simulator->instructionsSimulated) / 1000000.0) / seconds; // Calculate MIPS
}

void runWidget::startSingleStepMode() {
    // Change buttons and text to accomadate the new mode
    start_btn->setText("Step");
    description_lbl->clear();
    disconnect(start_btn, SIGNAL(clicked()), this, SLOT(startSim()));
    connect(start_btn, SIGNAL(clicked()), this, SLOT(nextStep()));

    // Start single step simulation
    nextStep();
}

void runWidget::nextStep() {
    if (simulator->isRunning()) {
        word instr = simulator->IM.read(simulator->cpu.PC);
        word thisPC = simulator->cpu.PC;
        simulator->cpu.doInstruction(simulator->cpu.getOpCode(instr), instr, simulator->DM, simulator->IM);
        std::stringstream stepInfo;
        stepInfo << "\n(" << simulator->instructionsSimulated << ")"
                 << simulator->cpu.getInstr(instr) << " @" << thisPC << " : "
                 << simulator->cpu.getRegisterFile()
                 << "next PC: " << simulator->cpu.PC;
        description_lbl->setText(description_lbl->text() + QString::fromStdString(stepInfo.str()));
    }
}
