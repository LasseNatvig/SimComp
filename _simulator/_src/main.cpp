#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<bitset>
#include<iomanip>
#include <utility>
#include "logger.h"
#include "utils.h"
#include "compSim.h"

using namespace std;

int main(int argc, char *argv[]) {
	string sasmPath; // Path to catalog for sasm files
	if (argc >= 2) // argv[0] is program name
		sasmPath = argv[1];
	else
		sasmPath = "./"; // Read from current directory
	ComputerSimulation sim("AdHoc16_V03");
	sim.load(selectProgram(sasmPath));
	char simMode = ' '; // Space is no mode

	clock_t t;
	while (simMode != 'q') {
		simMode = selectSimulationMode(sim);
		if (sim.isRunning()) {
				cout << "*** Starts simulation . . ." << endl;
				clock_t start = readTime();
				sim.runProgram();

				t = readTime() - start;
				cout << "*** Simulation ended" << endl;
				if (!sim.singleStepConsole())  // Time and MIPS rate does not make sense in singe step mode
						reportMIPS(t, sim.instructionsSimulated);
				if (sim.dump()) sim.dumpStats();
		}
	}
}
