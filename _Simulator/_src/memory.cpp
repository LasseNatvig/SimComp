
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
	/* Dump memory statistics */
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

void Memory::getStats(Isa& cpu, vector<std::string> &vec) const {
    /* Fills vec with stats, one item per line */
    stringstream ss;
    ss << "Memory stats for " << name << " is:";
    vec.push_back(ss.str());
    ss.str(string());

    ss << "Words used: " << dec << nextFreeLocation <<
        " Reads: " << memReads << " Writes: " << memWrites;
    vec.push_back(ss.str());
    ss.str(string());

    ss << "Memory dump for " << name << " is:" << hex;
    vec.push_back(ss.str());
    ss.str(string());

    if (type == DATA) {
        for (unsigned int i = 0; i < words.size(); i++) {
            ss << setw(5) << words[i] << " "; // TODO print as 0xffff, 10 pr. line, preceeded by address
            if (((i + 1) % 10) == 0) {
                vec.push_back(ss.str());
                ss.str(string());
            }
        }
    } else if (type == INSTR) {
        for (unsigned int i = 0; i < words.size(); i++) {
            ss << dec << i << " | " << hex << i << ": ";
            if (cpu.printInstr(words[i])) { // it is a SET instruction
                ss << " " << setw(5) << hex << words[i];
                vec.push_back(ss.str());
                ss.str(string());
                i++;
                ss << "\t" << words[i];
                vec.push_back(ss.str());
                ss.str(string());
            } else {
                  ss << " " << setw(5) << hex << words[i];
                  vec.push_back(ss.str());
                  ss.str(string());
            }
        }
    }
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
