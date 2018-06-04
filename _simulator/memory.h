#pragma once
#include<iostream>
#include<vector>
#include<string>
#include "isa.h"

enum memType { DATA, INSTR };
typedef uint16_t word;

class Isa;  // forward declaration

class Memory {
private:
	int size;
	unsigned int nextFreeLocation;
	long long memReads;
	long long memWrites;
	std::vector<word> words;
public:
	std::string name; // Avoid using using namespace in .h files
	memType type;
	Memory(std::string name, memType type);
	void dumpStats(Isa& cpu) const; 
	void resetStats();
	std::string getName();
	word getNextFreeLocation();
	void write(word addr, word w);
	word read(const word& addr);
};