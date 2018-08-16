#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "compSim.h"
#include "logger.h"

class Loader {
private:
	int lines;
	LogFile* logFile;
	void reportError(std::string errorMsg) const;
	word getOperand(std::string opStr) const;
	word getAddress(std::string opStr) const;
	word get3bitsConstant(std::string opStr) const;
	bool isLabel(std::string& s) const;
	void parseAlloc(Program& prog, std::string& name, std::string& type, std::string& size, Memory& DM);
	void parseConst(Program& prog, std::string& name, std::string& type, std::string& value, Memory& DM);
	void generateInstruction(Program& prog, std::string& label, std::string& instr, std::string operand[], Isa& cpu, Memory& IM);
public:
	std::vector<int> pcValue;
	Loader(LogFile* logFile);

	void load(std::string fileName, Program& prog, Isa& cpu, Memory& DM, Memory& IM); // Load file

};
