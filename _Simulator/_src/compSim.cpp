#include <time.h>
#include "compSim.h"

using namespace std;

ComputerSimulation::ComputerSimulation(string name, string loggerFilename) :
 	name(name), loggerFilename(loggerFilename),
	IM("Instruction Memory", INSTR, &logg), DM("Data Memory", DATA, &logg) // Initialize IM and DM
	{
	logg.open(loggerFilename); // Start logging

  cpu = new Isa(&logg);
  sasmLoader = new Loader(&logg);

	// Set up instruction statics table
	short i = 0;
	for (auto it = cpu->isaMap.begin(); it != cpu->isaMap.end(); it++)
		instStatsTable[it->second] = i++;
	reset();
	logg.write("ComputerSimulation " + name + " started at time ");
	logg.timeStamp();
}

ComputerSimulation::ComputerSimulation(string name) : ComputerSimulation(name, "logg.txt") {}

ComputerSimulation::~ComputerSimulation() {
	/* Delete statistics table */
	if (instStats != nullptr)
		delete[] instStats;
  if (cpu != nullptr)
    delete cpu;
  if (sasmLoader != nullptr)
    delete sasmLoader;

}

void ComputerSimulation::load(string name) {
	sasmLoader->load(name, sasmProg, *cpu, DM, IM);
}

void ComputerSimulation::reset() {
	/* Reset variables to default */
	instructionsSimulated = 0;
  currentMode = NOTRUNNING;
  resetStatistics();
}

void ComputerSimulation::resetStatistics() {
	/* Reset and cleanup of statistics */
	IM.resetStats();
	DM.resetStats();
	if (instStats != nullptr)
		delete[] instStats;
	instStats = new long long[cpu->getMaxNoInstructions()];
	for (short i = 0; i < cpu->getMaxNoInstructions(); i++) instStats[i] = 0;
}

void ComputerSimulation::run() {
	/*
	- Normal execution of program if currentMode == RUNNING or NOTRUNNING
	- Executes one step if currentMode == SINGLESTEP,
	 */
	cpu->PC = 0;
	do {
		word instr = IM.read(cpu->PC);
		short opCode = cpu->getOpCode(instr);
		instructionsSimulated++;
		if (opCode == HLT)
			setMode(NOTRUNNING);
		else {
			currentMode = RUNNING;
			cpu->doInstruction(opCode, instr, DM, IM);
		}
	} while (isRunning());
}
