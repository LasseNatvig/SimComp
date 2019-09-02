#include <string>
#include "../../_Simulator/_src/compSim.h"
#include "../../_Simulator/_src/logger.h"

const std::string continueMsg{ "....type any character and return to quit SimComp\n" };

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
