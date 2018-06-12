#include <string>
#include "../../_Simulator/_src/compSim.h"
#include "../../_Simulator/_src/logger.h"

class ConsoleUi {
public:
  ConsoleUi(std::string directory);
  ~ConsoleUi();
  void start(); // Starts console user interface

private:
  ComputerSimulation* sim;
  void step();
  void dumpStats();
};
