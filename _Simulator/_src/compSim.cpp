#include <time.h>
#include <sstream>
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
}

void ComputerSimulation::reset() {
    /* Reset simulation:
  - All variables to default
      Most important: Reset Memory
  */
    cpu->PC = 0;
    instructionsSimulated = 0;
    currentMode = NOTRUNNING;
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
    do {
        word instr = IM.read(cpu->PC);
        short opCode = cpu->getOpCode(instr);
        if (opCode == HLT) {
            instructionsSimulated++;
            setMode(NOTRUNNING);
        } else {
            setMode(RUNNING);
            instructionsSimulated++;
            instStats[instStatsTable[opCode]]++;
            cpu->doInstruction(opCode, instr, DM, IM);
        }
    } while (isRunning());
}

bool ComputerSimulation::step() {
    word instr = IM.read(cpu->PC);
    short opCode = cpu->getOpCode(instr);
    if (opCode == HLT) {
        instructionsSimulated++;
        setMode(NOTRUNNING);
        return false;
    }
    setMode(SINGLESTEP);
    instructionsSimulated++;
    instStats[instStatsTable[opCode]]++;
    cpu->doInstruction(opCode, instr, DM, IM);
    return true;
}

vector<string> ComputerSimulation::memoryDump(word fromAddr, word toAddr, memType memoryType) {
  vector<string> vec;
  stringstream ss;
  if (fromAddr > toAddr) {
    writeToLogg("memoryDump(): Invalid fromAddr > toAddr\n");
    return vec;
  }
  for (word i = fromAddr; i <= toAddr; i++) {
    if (memoryType == DATA) {
      if ((fromAddr < 0) || (toAddr > DM.words.size()-1)) return vec;
      ss << "0x" << hex << DM.words[i];
    } else if (memoryType == INSTR) {
      if ((fromAddr < 0) || (toAddr > IM.words.size()-1)) return vec;
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
