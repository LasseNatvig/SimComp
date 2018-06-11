
void ComputerSimulation::singleStep(short opCode, word instr) {
	/* Executes a single step (equal to executing one instruction) */
	// Uses call-by-value to show that opCode and instr cannot be changed

    word thisPC = cpu.PC;
    cpu.doInstruction(opCode, instr, DM, IM);
    if (singleStepMode_console) {
		cout << "(" << instructionsSimulated << ") after ";
		cpu.printInstr(instr);
		cout << " @" << thisPC << ":";
		cpu.printRegisterFile();
		cout << "next PC: " << cpu.PC << endl;

		char nextAction;
		nextAction = selectSingleStepAction();
		switch (nextAction) {
        case 'r': singleStepMode_console = false;
			  break;
		case 's': ; // Just continue
			  break;
		case 't':
			setRunning(false);
            singleStepMode_console = false;
			break;
        }
	}
}


void ComputerSimulation::dumpStats() {
	/* Dump current statistics */
	IM.dumpStats(cpu);
	DM.dumpStats(cpu);
	cout << "Instruction statistics:" << endl;
	for (auto it = instStatsTable.begin(); it != instStatsTable.end(); ++it) {
		cout << "Inst w/opcode: " << hex << it->first << " "
			<< dec << instStats[it->second] << endl;
	}
}

void Memory::dumpStats(Isa& cpu) const {
	/* Dump memory statistics */
	cout << "Memory stats for " << name << " is: \n"
		<< "Words used: " << dec << nextFreeLocation <<
		" Reads: " << memReads << " Writes: " << memWrites << endl;
	cout << "Memory dump for " << name << " is:" << hex << endl;
	if (type == DATA) {
		for (unsigned int i = 0; i < words.size(); i++) {
			cout << setw(5) << words[i] << " "; // TODO print as 0xffff, 10 pr. line, preceeded by address
			if (((i + 1) % 10) == 0) cout << endl;
		}
		cout << endl;
	}
	else if (type == INSTR) {
		for (unsigned int i = 0; i < words.size(); i++) {
			cout << dec << i << " | " << hex << i << ": ";
			if (cpu.printInstr(words[i])) { // it is a SET instruction
				cout << " " << setw(5) << hex << words[i] << endl;
				i++;
				cout << "\t" << words[i] << endl;
			}
			else
				cout << " " << setw(5) << hex << words[i] << endl;
			// if (((i + 1) % 10) == 0) cout << endl; // TODO 10 pr. line, later preceeded with address
		}
		cout << endl;
	}
	else
		cout << "ERROR TODO illegal type of memory";
}
