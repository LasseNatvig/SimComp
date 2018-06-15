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
  std::string directory; // Directory with *.sasm files
  std::string filename; // *.sasm filename
  void step();
  void dumpStats();
};
