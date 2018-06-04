#include <iomanip>
#include<string>
#include "compSim.h"
#include "memory.h"
#include "logger.h"
#include "config.h"
#include "utils.h"
#include "isa.h"
#include <time.h>
using namespace std;

ComputerSimulation::ComputerSimulation(string n) : name(n),
	IM("Instruction Memory", INSTR ), DM("Data Memory", DATA) {
	logg.open("SimCompLog.txt");
	short i = 0; // set up instruction statics table
	for (auto it = cpu.isaMap.begin(); it != cpu.isaMap.end(); it++)
		instStatsTable[it->second] = i++;
	reset();
	logg.write("ComputerSimulation " + name + " started at time ");
	logg.timeStamp();
}

void ComputerSimulation::load(string name) {
	sasmLoader.load(name, sasmProg, cpu, DM, IM, logg);
}

void ComputerSimulation::reset() {
	instructionsSimulated = 0;
	singleStepMode = false;
	dumpMode = false;
    running = true;
}

void ComputerSimulation::resetStatistics(const Isa& isa ) {
	IM.resetStats();
	DM.resetStats();
	if (instStats != nullptr)
		delete[] instStats;
	instStats = new long long[isa.maxNoInstructions];
	for (short i = 0; i < isa.maxNoInstructions; i++) instStats[i] = 0;
}

ComputerSimulation::~ComputerSimulation() {
	if (dumpMode) {
		IM.dumpStats(cpu);
		DM.dumpStats(cpu);
	}
	if (instStats != nullptr)
		delete[] instStats;
}

void ComputerSimulation::singleStep(short opCode, word instr) {
	// Uses call-by-value to show that opCode and instr cannot be changed
	word thisPC = cpu.PC;

	cpu.doInstruction(opCode, instr, DM, IM);
	if (singleStepMode) {
		cout << "(" << instructionsSimulated << ") after ";
		cpu.printInstr(instr);
		cout << " @" << thisPC << ":";
		cpu.printRegisterFile();
		cout << "next PC: " << cpu.PC << endl;

		char nextAction;
		nextAction = selectSingleStepAction();
		switch (nextAction) {
		case 'r': singleStepMode = false;
			break;
		case 's': ; // just continue
			break;
		case 't': setRunning(false);
			singleStepMode = false;
			break;
		}
	}
}

void ComputerSimulation::runProgram() {
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
	IM.dumpStats(cpu);
	DM.dumpStats(cpu);
	cout << "Instruction statistics:" << endl;
	for (auto it = instStatsTable.begin(); it != instStatsTable.end(); ++it) {
		cout << "Inst w/opcode: " << hex << it->first << " "
			<< dec << instStats[it->second] << endl;
	}
}
