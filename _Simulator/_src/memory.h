#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "isa.h"
#include "logger.h"

enum memType { DATA, INSTR };
typedef uint16_t word;

class Isa;  // Forward declaration

class Memory {
private:
	LogFile* logFile;
	void reportError(std::string errorMsg) const;

	unsigned int size;
	unsigned int nextFreeLocation;
	long long memReads;
	long long memWrites;
	std::vector<word> words;

public:
	std::string name; // Avoid using using namespace in .h files
	memType type;

	/* Constructor(s) */
	Memory(std::string name, memType type, LogFile* logFile);

	/* Core functionality */
	void resetStats(); // Reset statistics (Sets varibles to default values)
  void getStats(Isa& cpu, std::vector<std::string> &vec) const;
	void write(word addr, word w); // Write to memory
	word read(const word& addr); // Read momory at adress "addr"

	/* Get funcitons */
	std::string getName();
	word getNextFreeLocation();
};
