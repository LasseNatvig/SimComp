#pragma once
#include <string>
#include "logger.h"
#include "memory.h"
#include "isa.h"
#include "program.h"
#include "loader.h"


class ComputerSimulation {
private:
	std::string name;
	bool running = false;
	bool dumpMode = false;
	bool singleStepMode = false;

public:
	LogFile logg;
	Memory IM; // Instruction memory
	Memory DM; // Data memory

	Loader sasmLoader; // Loader responsible for parsing and loading sasm-file to program (asm is C++ keyword, cannot be used)
	Program sasmProg; // SimComp ASM (sasm) program
	Isa cpu; // Instance of ISA, responsible for execution of instructions
	long long instructionsSimulated = 0; // Instructions simulated count
	std::map<word, short> instStatsTable; // Statistics table
	long long* instStats = nullptr;  // Size given by Isa member constant

	/* Destructor and constructor(s) */
	~ComputerSimulation();
	ComputerSimulation(std::string name);

	/* Core functionality */
	void load(std::string name); // Loads(and parses) sasm-file named "name" to sasmProg
	void reset(); // Reset varibles (sets simulator parameters to default values)
	void singleStep(short opCode, word instr); // Executes single instruction of program
	void runProgram(); // Normal execution of program
	void dumpStats(); // Dump statistics
	void resetStatistics(const Isa& isa); // Reset statistics

	/* Get/set functions */
	void setRunning(bool mode) { running = mode; }
	bool isRunning() { return running; }
	bool dump() { return dumpMode; }
	void setDumpMode(bool mode) { dumpMode = mode; }
	bool singleStep() { return singleStepMode; }
	void setSingleStepMode(bool mode) { singleStepMode = mode; }
};
