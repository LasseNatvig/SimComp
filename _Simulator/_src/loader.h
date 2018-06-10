#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "compSim.h"

class Loader {
private:
	int lines;
	void errorLoad(std::string msg);
	word getOperand(std::string opStr);
	word getAddress(std::string opStr);
	word get3bitsConstant(std::string opStr);
	bool isLabel(std::string& s);
	void parseAlloc(Program& prog, std::string& name, std::string& type, std::string& size, Memory& DM);
	void parseConst(Program& prog, std::string& name, std::string& type, std::string& value, Memory& DM, LogFile& logg);
	void generateInstruction(Program& prog, std::string& label, std::string& instr, std::string operand[], Isa& cpu, Memory& IM, LogFile& logg);
public:
	Loader();
	void load(std::string fileName, Program& prog, Isa& cpu, Memory& DM, Memory& IM, LogFile& logg);
};