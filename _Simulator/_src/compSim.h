#pragma once
#include <string>
#include <vector>
#include "logger.h"
#include "memory.h"
#include "isa.h"
#include "program.h"
#include "loader.h"

/* Possible modes to run computer simulation in: */
enum Mode {
	RUNNING,
	SINGLESTEP,
	NOTRUNNING
};

class ComputerSimulation {
public:
	Isa* cpu; // Instance of ISA, responsible for execution of instructions

	/* Destructor and constructor(s) */
	~ComputerSimulation();
	ComputerSimulation(std::string name, std::string loggerFilename);
	ComputerSimulation(std::string name);

	/* Core functionality */
	void load(std::string name); // Loads(and parses) sasm-file named "name" to sasmProg
	void reset(); // Reset varibles (sets simulator parameters to default values)
	void run(); 	// Normal execution of program if currentMode == RUNNING or NOTRUNNING
	bool step(); // Executes one step, returns true if instruction != HLT
	void writeToLogg(std::string message) { logg.write(message); } // Write to logg
	void resetStatistics(); // Reset statistics
	std::vector<std::string> memoryDump(word fromAddr, word toAddr, memType memory);

	/* Get functions */
	word getPC() { return cpu->PC; }
	long long getInstructionsSimulated() const { return instructionsSimulated; }
	bool isRunning() const { return (!(currentMode == NOTRUNNING)); }
	bool singleStep() const { return (currentMode == SINGLESTEP); }
	bool program() const { return sasmProg.valid; } // Checks if program is succesfully loaded
	word getInstr(word PC) { return IM.read(PC); }
	Mode getMode() const { return currentMode; }
	std::string getName() const { return name; }

	/* Set functions */

	void setPC(word PC) { cpu->PC = PC; }

	/* Core variables */
	long long* instStats = nullptr;  // Used to count occurences of instructions (Size given by Isa member constant)
	std::map<word, short> instStatsTable; // Statistics table
	Memory IM; // Instruction memory
	Memory DM; // Data memory

private:
	std::string name; // Name of cumputer simulation
	std::string loggerFilename; // Logg filename

	/* Core variables */
	LogFile logg; // Logger file
	Loader* sasmLoader; // Loader responsible for parsing and loading sasm-file to program
	Program sasmProg; // SimComp ASM (sasm) program
	Mode currentMode = NOTRUNNING; // Current simulation mode
	long long instructionsSimulated = 0; // Instructions simulated count
	void setMode(Mode mode) { currentMode = mode; }

};
