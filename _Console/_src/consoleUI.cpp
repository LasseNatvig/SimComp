#include <iomanip>
#include <vector>
#include "consoleUI.h"
#include "utils.h"

ConsoleUi::ConsoleUi(std::string directory) : directory(directory){
    sim = new ComputerSimulation("AdHoc16_V03", "logg.txt");
}

ConsoleUi::~ConsoleUi() {
  delete sim;
  sim = nullptr;
}

void ConsoleUi::start() {
  filename = selectProgram(directory);
  sim->load(directory+filename);
  char simMode = selectSimulationMode(*sim);
  clock_t t;
  clock_t start;
  while (simMode != 'q') {
    std::cout << "*** Starts simulation . . ." << std::endl;
    switch (simMode) {
      case 's':
        step();
        break;
      case 'r':
      case 'd':
        start = readTime();
        sim->run();
        break;
    }
    t = readTime() - start;
    sim->load(directory+filename);
    simMode = selectSimulationMode(*sim);
  }
}

void ConsoleUi::step() {
  word thisPC = sim->cpu->PC;
  if (!(sim->step())) return;
  std::cout << "(" << sim->getInstructionsSimulated() << ") after "
       << sim->cpu->disAssembly(sim->getInstr(sim->cpu->PC)) << " @" << thisPC << ":"
       << sim->cpu->getRegisterFile() << "next PC: "
       << sim->cpu->PC << std::endl;

  char nextAction;
  nextAction = selectSingleStepAction();
  switch (nextAction) {
      case 'r': sim->run();
      break;
  case 's': step(); // Just continue
      break;
  case 't':
    break;
  }
}

void ConsoleUi::dumpStats() {
	/* Dump memory statistics */
  std::vector<std::string> imStats = sim->IM.getStats(*sim->cpu);
  std::vector<std::string> dmStats = sim->DM.getStats(*sim->cpu);
	for (auto &line : imStats)
    std::cout << line << std::endl;
  for (auto &line : dmStats)
    std::cout << line << std::endl;
	std::cout << "Instruction statistics:" << std::endl;
	for (auto it = sim->instStatsTable.begin(); it != sim->instStatsTable.end(); ++it) {
		std::cout << "Inst w/opcode: " << std::hex << it->first << " "
			<< std::dec << sim->instStats[it->second] << std::endl;
	}
}
