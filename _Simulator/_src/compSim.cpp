#include <time.h>
#include <sstream>
#include <algorithm>
#include "compSim.h"

using namespace std;

ComputerSimulation::ComputerSimulation(string name, string loggerFilename) :
    name(name), loggerFilename(loggerFilename),
    IM("Instruction Memory", INSTR, &logg), DM("Data Memory", DATA, &logg) // Initialize IM and DM
{
    logg.open(loggerFilename); // Start logging
    cpu = new Isa(&logg);
    sasmLoader = new Loader(&logg);

    // Set up instruction statics table
    short i = 0;
    for (auto it = cpu->isaMap.begin(); it != cpu->isaMap.end(); it++)
        instStatsTable[it->second] = i++;
    logg.write("ComputerSimulation " + name + " started at time ");
    logg.timeStamp();
}

ComputerSimulation::ComputerSimulation(string name) : ComputerSimulation(name, "logg.txt") {}

ComputerSimulation::~ComputerSimulation() {
    /* Delete statistics table */
    if (instStats != nullptr)
        delete[] instStats;
    if (cpu != nullptr)
        delete cpu;
    if (sasmLoader != nullptr)
        delete sasmLoader;

}

void ComputerSimulation::load(string name) {
    reset();
    sasmLoader->load(name, sasmProg, *cpu, DM, IM);
    breakpoints.resize(IM.words.size(),0);
}

void ComputerSimulation::reset() {
    /* Reset simulation:
  - All variables to default
      Most important: Reset Memory
  */
    cpu->PC = 0;
    instructionsSimulated = 0;
    currentMode = NOTRUNNING;
    sasmProg.valid = false;
    breakpoints.clear();
    IM.reset();
    DM.reset();
    resetStatistics();
}

void ComputerSimulation::resetStatistics() {
    /* Reset and cleanup of statistics */
    IM.resetStats();
    DM.resetStats();
    if (instStats != nullptr)
        delete[] instStats;
    instStats = new long long[cpu->getMaxNoInstructions()];
    for (short i = 0; i < cpu->getMaxNoInstructions(); i++) instStats[i] = 0;
}

void ComputerSimulation::run() {
    /* Normal execution of program */
    setMode(RUNNING);
    do {
      nextInstruction();
    } while (isRunning());
}

bool ComputerSimulation::step() {
    setMode(SINGLESTEP);
    return nextInstruction();
}

bool ComputerSimulation::next() {
  bool ret;
  setMode(RUNNING);
   do {
    ret = nextInstruction();
  } while (isRunning() && !(breakpoints[cpu->PC]));
  return ret;
}

void ComputerSimulation::setBreakpoints(const vector<int> &lineBreakpoints) {
    breakpoints.clear();
    breakpoints.resize(IM.words.size(),0);
    for (auto& lineBreakpoint : lineBreakpoints) {
        if (sasmLoader->pcValue[lineBreakpoint-1] > -1)
            breakpoints[sasmLoader->pcValue[lineBreakpoint-1]] = 1;
    }
}

vector<string> ComputerSimulation::memoryDump(word fromAddr, word toAddr, memType memoryType) {
  vector<string> vec;
  stringstream ss;
  if (fromAddr > toAddr) {
    writeToLogg("memoryDump(): Invalid fromAddr > toAddr\n");
    return vec;
  } else if (fromAddr < 0) {
    writeToLogg("memoryDump(): Invalid fromAddr < 0\n");
    return vec;
  }
  for (word i = fromAddr; i <= toAddr; i++) {
    if (memoryType == DATA) {
      if (!DM.words.size()) return vec;
      if (i > DM.words.size()-1) return vec;
      ss << "0x" << hex << DM.words[i];
    } else if (memoryType == INSTR) {
      if (!IM.words.size()) return vec;
      if (i > IM.words.size()-1) return vec;
      ss << cpu->disAssembly(IM.words[i]);
      if (((IM.words[i] & 0b1111111000000000) >> 9) == SET)
        ss << " " << IM.words[++i];
    } else {
      writeToLogg("Invalid memory type used.\n");
      return vec;
    }
    vec.push_back(ss.str());
    ss.str(string());
  }
  return vec;
}

bool ComputerSimulation::nextInstruction() {
  word instr = IM.read(cpu->PC);
  short opCode = cpu->getOpCode(instr);
  if (opCode == HLT) {
      instructionsSimulated++;
	  instStats[instStatsTable[opCode]]++;
      setMode(NOTRUNNING);
      return false;
  }
  instructionsSimulated++;
  instStats[instStatsTable[opCode]]++;
  cpu->doInstruction(opCode, instr, DM, IM);
  return true;
}
