
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include "memory.h"

using namespace std;

Memory::Memory(string name, memType type) : name(name), type(type) { 
		nextFreeLocation = 0;
		memReads = memWrites = 0;
}

void Memory::resetStats() {
	memReads = memWrites = 0;
}

void Memory::dumpStats(Isa& cpu) const {
	cout << "Memory stats for " << name << " is: \n"
		<< "Words used: " << dec << nextFreeLocation <<   
		" Reads: " << memReads << " Writes: " << memWrites << endl;
	cout << "Memory dump for " << name << " is:" << hex << endl;
	if (type == DATA) {
		for (unsigned int i = 0; i < words.size(); i++) {
			cout << setw(5) << words[i] << " "; // TODO print as 0xffff, 10 pr. line, preceeded by address 
			if (((i + 1) % 10) == 0) cout << endl;
		}
		cout << endl;
	}
	else if (type == INSTR) {
		for (unsigned int i = 0; i < words.size(); i++) {
			cout << dec << i << " | " << hex << i << ": ";
			if (cpu.printInstr(words[i])) { // it is a SET instruction
				cout << " " << setw(5) << hex << words[i] << endl;
				i++; 
				cout << "\t" << words[i] << endl;
			}
			else
				cout << " " << setw(5) << hex << words[i] << endl;
			// if (((i + 1) % 10) == 0) cout << endl; // TODO 10 pr. line, later preceeded with address 
		}
		cout << endl;
	}
	else
		cout << "ERROR TODO illegal type of memory";
}

word Memory::getNextFreeLocation() { 
	words.push_back(0); // allocate one word, zeroed by default
	return nextFreeLocation++; 
}

void Memory::write(word addr, word w) { 
	if (addr > words.size()) {
		cout << "Memory write error: illegal address " + to_string(addr); // TODO print in hex
		exit(-1);
	}
	else {
		words[addr] = w;
		memWrites++;
	}
}

word Memory::read(const word& addr) {  
		if (addr > words.size()) {
		cout << "Memory read error: illegal address " + to_string(addr); // TODO print in hex
		exit(-1);
	}
	else {
		memReads++;
		return words[addr];
	}
}

string Memory::getName() { return name; }