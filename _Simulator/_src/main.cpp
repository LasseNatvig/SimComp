#include <iostream>
#include "compSim.h"


int main(int argc, char *argv[]) {
	ComputerSimulation sim("hella");
	sim.load("../_sasm/test0.sasm");
	sim.run();
	std::cout << sim.getInstructionsSimulated() << std::endl;
	std::cout << sim.cpu->getInstr(ADD) << std::endl;
	std::cout << ADD << std::endl;
	return 0;
}
