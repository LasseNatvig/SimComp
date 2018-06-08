#pragma once
#include <map>
#include <string>
#include "memory.h"

class Program {
public:
	std::map<std::string, word> symbolTable; // Maps labels and variable names to addresses
	std::map<std::string, word> intConstants; // Integer constants, all of size word
	std::map<std::string, std::string> strConstants; // String constants, of any size
	std::map<std::string, word> variableSizes; // Store size of variables in no of words
	Program();
};
