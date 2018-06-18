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


public:
	std::vector<word> words;

	std::string name; // Avoid using using namespace in .h files
	memType type;

	/* Constructor(s) */
	Memory(std::string name, memType type, LogFile* logFile);

	/* Core functionality */
	void reset(); // Resets Memory: clear words, size = nextFreeLocation = 0  and call resetStats().
	void resetStats(); // Reset statistics (Sets varibles to default values)
  std::vector<std::string> getStats(Isa& cpu) const; // Returns vector with stats, one item per line
	void write(word addr, word w); // Write to memory
	word read(const word& addr); // Read word at address "addr"
	long long getMemoryRead() const { return memReads; }
	long long getMemoryWrites() const { return memWrites; }

	/* Get funcitons */
	std::string getName();
	word getNextFreeLocation();
};
