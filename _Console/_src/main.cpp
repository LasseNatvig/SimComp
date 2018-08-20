#include <string>
#include "consoleUI.h"
#include "../../_Simulator/_src/compSim.h"
#include "../../_Simulator/_src/logger.h"

int main(int argc, char *argv[]) 
	try {
		std::string sasmPath; // Path to catalog for sasm files
		if (argc >= 2) { // argv[0] is program name
			sasmPath = argv[1];
		}
		else {
			sasmPath = "../../../../_Simulator/_sasm/"; // NOTE works when double-clicking on .exe file in x64/Release or Debug directory 
			// sasmPath = "../../_Simulator/_sasm/"; // NOTE works when running from debugger in MS-VS or running release -mode ...
													 // --- to avoid having two configs an absolute path as command argument in MS-V-project (Local for Lasse)
		}
		ConsoleUi ui(sasmPath);
		ui.start();
		return 0;
	}
catch (std::exception& e) {
	std::cerr << "Simcomp exception: " << e.what() << std::endl;
	std::cerr << ".... type any character and return to quit SimComp\n";
	char c;
	std::cin >> c;
	return 1;
}
catch (...) {
	std::cerr << "Simcomp exception ";
	std::cerr << ".... type any character and return to quit SimComp\n";
	char c;
	std::cin >> c;
	return 2;
}