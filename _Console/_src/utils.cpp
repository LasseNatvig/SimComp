#include<iostream>
#include<string>
#include <vector>
#include "utils.h"
#include "time.h"
#include "config.h"

using namespace std;

vector<string> menuSim{
	"r --- run program",
	"s --- single step program",
	"d --- run program and dump simulation statistics",
	"q --- quit simulator"
};

vector<string> menuStep{ "s - step, ", "r - run, ", "t - terminate execution:"};

string textMenuS(string question, const vector<string>& menu) {
	/* Variant of textMenu that returns the selected string */
	cout << endl << question << endl;
	for (unsigned int i = 0; i < menu.size(); i++) {
		cout << i << ": " << menu[i] << endl;
	}
	unsigned short choice;
	cin >> choice;
	if ((choice >= 0) && (choice < menu.size()))
		return (menu[choice]);
	else {
		cerr << "****** User error: illegal option selected \n";
		exit(-1);
		return(nullptr);
	}
}

char textMenuC(string question, vector<string>& menu, bool compact) {
	/* Variant of textMenu that returns the first character in the string */
	cout << question;
	if (compact) cout << " "; else cout << endl;
	for (unsigned int i = 0; i < menu.size(); i++) {
		cout << menu[i];
		if (compact) cout << " "; else cout << endl;
	}
	char choice;
	cin >> choice;
	for (unsigned int i = 0; i < menu.size(); i++) {
		if (choice == menu[i][0])
			return (menu[i][0]);
	}
	cerr << "****** User error: illegal option selected \n";
	exit(-1);
	return(' ');
}

char selectSimulationMode(ComputerSimulation& sim) {
	char mode = ' ';
	mode = textMenuC("\nSelect simulation mode by typing a single letter:", menuSim, false);
	sim.resetStatistics();
	return(mode);
}

char selectSingleStepAction() {
	return(textMenuC("Next single step action:", menuStep, true));
}

string selectProgram(string path) {
	return(path +
		 textMenuS("Select assembler program to run:", getSASMfiles(path)));
}

clock_t readTime() { return clock(); }

void reportMIPS(clock_t ticks, long long instructions) {
	float seconds = ((float) ticks) / CLOCKS_PER_SEC;
	cout.setf(ios::fixed); cout.setf(ios::showpoint); cout.precision(2);
	if (seconds > 0.0) {
		cout << "Total of " << instructions
			<< " instructions\n@ " << seconds << " seconds" << endl;
		cout << "@ " << (static_cast<double>(instructions) / 1000000.0) / seconds << " MIPS\n";
	}
	else
		cout << "Total of " << instructions << " instructions executed.\n";
}
