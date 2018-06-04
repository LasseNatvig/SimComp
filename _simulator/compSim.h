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
	Memory IM; 
	Memory DM; 

	Loader sasmLoader; // asm is C++ keyword, cannot be used
	Program sasmProg; // SimComp ASM (sasm) program
	Isa cpu; 
	long long instructionsSimulated = 0;
	std::map<word, short> instStatsTable;
	long long* instStats = nullptr;  // Size given by Isa member constant
		
	void load(std::string name);
	void reset();
	void setRunning(bool mode) { running = mode; }
	bool isRunning() { return running; }
	bool dump() { return dumpMode; }
	void setDumpMode(bool mode) { dumpMode = mode; }
	bool singleStep() { return singleStepMode; }
	void setSingleStepMode(bool mode) { singleStepMode = mode; }

	void resetStatistics(const Isa& isa);
	~ComputerSimulation();
	ComputerSimulation(std::string name);
	void singleStep(short opCode, word instr);
	void runProgram();
	void dumpStats();
};