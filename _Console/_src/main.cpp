#include <string>
#include "consoleUI.h"
#include "../../_Simulator/_src/compSim.h"
#include "../../_Simulator/_src/logger.h"

int main(int argc, char *argv[]) {
	std::string sasmPath; // Path to catalog for sasm files
	if (argc >= 2) // argv[0] is program name
		sasmPath = argv[1];
	else {
		sasmPath = "../../_Simulator/_sasm/"; // TODO-LN Is where I found sasmfiles
	}
	ConsoleUi ui(sasmPath);
	ui.start();
	return 0;
}