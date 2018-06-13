
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include "memory.h"

using namespace std;

Memory::Memory(string name, memType type, LogFile* logFile) : name(name), type(type),
    logFile(logFile) {
    nextFreeLocation = 0;
    memReads = memWrites = 0;
}

void Memory::reportError(string errorMsg) const {
    logFile->write("MEMORY ERROR: NAME=" + name + " MESSAGE: " + errorMsg);
    logFile->timeStamp();
    logFile->write("\n");
}

void Memory::resetStats() {
    memReads = memWrites = 0;
}

void Memory::reset() {
    size = nextFreeLocation = 0;
    words.clear();
    resetStats();
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
            if (cpu.disAssembly(words[i]).substr(0,2) == "SET") { // it is a SET instruction
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
        reportError("write error: illegal address " + to_string(addr)); // HANLE ERROR TODO print in hex
        exit(-1);
    }
    else {
        words[addr] = w;
        memWrites++;
    }
}

word Memory::read(const word& addr) {
    if (addr > words.size()) {
        reportError("Read error: illegal address " + to_string(addr)); // TODO print in hex
        exit(-1);
    }
    else {
        memReads++;
        return words[addr];
    }
}

string Memory::getName() { return name; }
