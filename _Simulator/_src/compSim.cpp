#include <iomanip>
#include <string>
#include <time.h>
#include "compSim.h"
#include "memory.h"
#include "logger.h"
#include "config.h"
#include "utils.h"
#include "isa.h"


using namespace std;


ComputerSimulation::ComputerSimulation(string n) : name(n),
	IM("Instruction Memory", INSTR ), DM("Data Memory", DATA) // Initialize IM and DM
	{
	logg.open("SimCompLog.txt"); // Start logging to file "SimCompLog.txt"

	/* Set up instruction statics table */
	short i = 0;
	for (auto it = cpu.isaMap.begin(); it != cpu.isaMap.end(); it++)
		instStatsTable[it->second] = i++;
	reset();
	logg.write("ComputerSimulation " + name + " started at time ");
	logg.timeStamp();
}

ComputerSimulation::~ComputerSimulation() {
	/* Dump statistics and delete statistics table */
	if (dumpMode) {
		IM.dumpStats(cpu);
		DM.dumpStats(cpu);
	}
	if (instStats != nullptr)
		delete[] instStats;
}

void ComputerSimulation::load(string name) {
	sasmLoader.load(name, sasmProg, cpu, DM, IM, logg);
}

void ComputerSimulation::reset() {
	/* Reset variables to default */
	instructionsSimulated = 0;
  singleStepMode_console = false;
  singleStepMode_gui = false;
	dumpMode = false;
  running = true;
}

void ComputerSimulation::resetStatistics(const Isa& isa ) {
	/* Reset and cleanup of statistics */
	IM.resetStats();
	DM.resetStats();
	if (instStats != nullptr)
		delete[] instStats;
	instStats = new long long[isa.maxNoInstructions];
	for (short i = 0; i < isa.maxNoInstructions; i++) instStats[i] = 0;
}

void ComputerSimulation::singleStep(short opCode, word instr) {
	/* Executes a single step (equal to executing one instruction) */
	// Uses call-by-value to show that opCode and instr cannot be changed

    word thisPC = cpu.PC;
    cpu.doInstruction(opCode, instr, DM, IM);
    if (singleStepMode_console) {
		cout << "(" << instructionsSimulated << ") after ";
		cpu.printInstr(instr);
		cout << " @" << thisPC << ":";
		cpu.printRegisterFile();
		cout << "next PC: " << cpu.PC << endl;

		char nextAction;
		nextAction = selectSingleStepAction();
		switch (nextAction) {
        case 'r': singleStepMode_console = false;
			break;
		case 's': ; // Just continue
			break;
		case 't':
			setRunning(false);
            singleStepMode_console = false;
			break;
        }
	}
}

void ComputerSimulation::runProgram() {
	/* Normal execution of program */
	cpu.PC = 0;
	do {
		word instr = IM.read(cpu.PC);
		short opCode = cpu.getOpCode(instr);
		if (dumpMode) instStats[instStatsTable[opCode]]++;
		instructionsSimulated++;

		if (opCode == HLT) {
			setRunning(false);
		}
		else singleStep(opCode, instr);
	} while (isRunning());
}

void ComputerSimulation::dumpStats() {
	/* Dump current statistics */
	IM.dumpStats(cpu);
	DM.dumpStats(cpu);
	cout << "Instruction statistics:" << endl;
	for (auto it = instStatsTable.begin(); it != instStatsTable.end(); ++it) {
		cout << "Inst w/opcode: " << hex << it->first << " "
			<< dec << instStats[it->second] << endl;
	}
}
