#include <iostream>
#include <string>
#include <bitset>
#include <iomanip>
#include "isa.h"
#include "memory.h"

using namespace std;

Isa::Isa() {
	isaMap = {  // TODO - later versions, should be for one instance, not the whole class
		{ "ADD", ADD },
		{ "ADI", ADI },
		{ "BRZ", BRZ },
		{ "HLT", HLT },
		{ "JMP", JMP },
		{ "LD",  LD },
		{ "LDI", LDI },
		{ "SET", SET },
		{ "SHL", SHL },
		{ "ST",  ST },
		{ "SUB", SUB }
	};
}

string Isa::disAssembly(word instr) {
	/* Returns string representation of instruction if found. */
	string found = "not found";
	instr = instr >> 9;
	for (auto it = isaMap.begin(); it != isaMap.end(); it++)
		if (it->second == instr)
			found = it->first;
	return found;
}

void Isa::printRegisterFile() {
	/* Prints content of all registers */
	for (int i = 0; i < noOfRegisters; i++)
		cout << setw(4) << hex << R[i] << " ";
	cout << dec;
}

bool Isa::printInstr(word machineInstr) {
		/* Prints mnemonic for instrucion */ // TODO Make use of the disAssembly function?
		word opCode = machineInstr & 0b1111111000000000;
		word theRest = machineInstr & 0b0000000111111111;
		bitset<9> bits;
		bits = theRest;
		string mnemonic;
		// TODO can be made faster by using a reverse map, but performance is not of interest here
		for (auto it = isaMap.begin(); it != isaMap.end(); it++)
			if (it->second == (opCode >> 9))
				mnemonic = it->first;
		cout << mnemonic << " " << bits;
		return (mnemonic == "SET");
}

short Isa::getOpCode(const word& w) {
	return ((w & 0b1111111000000000) >> 9);
}

// TODO make one get-function for all registers
word Isa::getRegNo1(const word& instr) {
	return((0b0000000111000000 & instr) >> 6);
}

word Isa::getRegNo2(const word& instr) {
	return((0b0000000000111000 & instr) >> 3);
}

word Isa::getRegNo3(const word& instr) {
	return(0b0000000000000111 & instr);
}

word Isa::getImmediate(const word& instr) {
	return (0b0000000000000111 & instr);
}

word Isa::getAdressOffset(const word& instr) {
	word AL = 0b0000000111000000 & instr;
	word AR = 0b0000000000000111 & instr;
	return ((AL >> 3) | AR);
}

void Isa::doInstruction(const short& opCode, const word& instr, Memory& DM, Memory& IM) {
	/* Executes instruction */
	switch (opCode) {
	case ADD:
		R[getRegNo1(instr)] = R[getRegNo2(instr)] + R[getRegNo3(instr)];
		PC++;
		break;
	case ADI:
		R[getRegNo1(instr)] = R[getRegNo2(instr)] + getImmediate(instr);
		PC++;
		break;
	case SHL:
		R[getRegNo1(instr)] = R[getRegNo3(instr)] << 1;
		PC++;
		break;
	case SUB:
		R[getRegNo1(instr)] = R[getRegNo2(instr)] - R[getRegNo3(instr)];
		PC++;
		break;
	case BRZ:
		if (R[getRegNo2(instr)] == 0)
			PC += getAdressOffset(instr);
		else
			PC++;
		break;
	case JMP:
		PC = R[getRegNo2(instr)];
		break;
	case LDI:
		R[getRegNo1(instr)] = getImmediate(instr);
		PC++;
		break;
	case LD:
		R[getRegNo1(instr)] = DM.read(R[getRegNo2(instr)]);
		PC++;
		break;
	case SET:
		PC++;
		R[getRegNo1(instr)] = IM.read(PC);
		PC++;
		break;
	case ST:
		DM.write(R[getRegNo1(instr)], R[getRegNo2(instr)]);
		PC++;
		break;
	default:
		printInstr(instr);
		cout << "Error: unimplemented instruction found at PC: " << PC
			<< " instr: " << hex << instr << "-- will exit";
		system("Pause");
		exit(-1);
	}
}
