#pragma once
#include <map>
#include <string>
#include "memory.h"

class Program {
public:
	std::map<std::string, word> symbolTable; // maps labels and variable names to addresses
	std::map<std::string, word> intConstants; // integer constants, all of size word
	std::map<std::string, std::string> strConstants; // string constants, of any size 
	std::map<std::string, word> variableSizes; // store size of variables in no of words
	Program();
};