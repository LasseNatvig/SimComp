#include <iomanip>
#include <vector>
#include "consoleUI.h"
#include "utils.h"

ConsoleUi::ConsoleUi(std::string directory) {
    sim = new ComputerSimulation("AdHoc16_V03", "logg.txt");
    sim->load(selectProgram(directory));
}

void ConsoleUi::start() {
  char simMode = ' '; // Space is no mode
  clock_t t;
  while (simMode != 'q') {
    simMode = selectSimulationMode(*sim);
    if (sim->isRunning()) {
        std::cout << "*** Starts simulation . . ." << std::endl;
        clock_t start = readTime();
        while (sim->isRunning()) {
          switch (sim->getMode()) {
            case RUNNING:
              sim->run();
              break;
            case SINGLESTEP:
              step();
              break;
          }
        }

        t = readTime() - start;
        std::cout << "*** Simulation ended" << std::endl;
        if (!(simMode == 's'))  // Time and MIPS rate does not make sense in singe step mode
            reportMIPS(t, sim->getInstructionsSimulated());
        if (simMode == 'd')
          dumpStats();
    }
  }
}

void ConsoleUi::step() {
  word thisPC = sim->cpu->PC;
  sim->step();
  std::cout << "(" << sim->getInstructionsSimulated() << ") after "
       << sim->cpu->getInstr(sim->getInstr()) << " @" << thisPC << ":"
       << sim->cpu->getRegisterFile() << "next PC: "
       << sim->cpu->PC << std::endl;

  char nextAction;
  nextAction = selectSingleStepAction();
  switch (nextAction) {
      case 'r': sim->setMode(RUNNING);
      break;
  case 's': // Just continue
      break;
  case 't': sim->setMode(NOTRUNNING);
    break;
  }
}

void ConsoleUi::dumpStats() {
	/* Dump memory statistics */
  std::vector<std::string> imStats;
  std::vector<std::string> dmStats;
  sim->IM.getStats(*sim->cpu, imStats);
  sim->DM.getStats(*sim->cpu, dmStats);
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
