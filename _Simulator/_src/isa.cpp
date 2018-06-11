#include <sstream>
#include <iostream>
#include <string>
#include <bitset>
#include <iomanip>
#include "isa.h"
#include "memory.h"

using namespace std;

Isa::Isa(LogFile* logFile) : logFile(logFile) {
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

string Isa::getRegisterFile() const {
    /* Returns register file as std::string */
    string regFile = "";
    for (int i = 0; i < noOfRegisters; i++)
        regFile += getRegister(i) + " ";
    return regFile;
}

string Isa::getRegister(int registerNum) const {
    stringstream stream;
    stream <<  setw(4) <<  hex << R[registerNum] << " ";
    stream << dec;
    return stream.str();
}

string Isa::getInstr(word machineInstr) const {
    /* Returns instruction opcode as mnemonic, and the rest as bits. */
    word opCode = machineInstr & 0b1111111000000000;
    bitset<9> bits = machineInstr & 0b0000000111111111;
    string mnemonic;
    // TODO can be made faster by using a reverse map, but performance is not of interest here
    for (auto it = isaMap.begin(); it != isaMap.end(); it++) {
        if (it->second == (opCode >> 9)) {
            mnemonic = it->first;
            break;
         }
    }
    stringstream ret;
    ret << mnemonic << " " << bits;
    return ret.str();
}

word Isa::getOpCode(const word& w) const {
	return ((w & 0b1111111000000000) >> 9);
}

word Isa::getReg(const word& instr, unsigned int regNo) const {
	switch (regNo) {
		case 1:
			return ((0b0000000111000000 & instr) >> 6);
		case 2:
			return ((0b0000000000111000 & instr) >> 3);
		case 3:
			return (0b0000000000000111 & instr);
		default:
			stringstream error;
			error << "Register number: " << regNo << " not found.";
			reportError(error.str());
			return 0;
	}
}

word Isa::getImmediate(const word& instr) const {
	return (0b0000000000000111 & instr);
}

word Isa::getAdressOffset(const word& instr) const {
	word AL = 0b0000000111000000 & instr;
	word AR = 0b0000000000000111 & instr;
	return ((AL >> 3) | AR);
}

void Isa::reportError(string errorMsg) const {
	logFile->write("ISA ERROR: " + errorMsg);
	logFile->timeStamp();
	logFile->write("\n");
}

// Spørsmål til Lasse: Er register HARDKODET??
void Isa::doInstruction(const short& opCode, const word& instr, Memory& DM, Memory& IM) {
	/* Executes instruction */
	switch (opCode) {
	case ADD:
		R[getReg(instr, 1)] = R[getReg(instr, 2)] + R[getReg(instr, 3)];
		PC++;
		break;
	case ADI:
		R[getReg(instr, 1)] = R[getReg(instr, 2)] + getImmediate(instr);
		PC++;
		break;
	case SHL:
		R[getReg(instr,1)] = R[getReg(instr, 3)] << 1;
		PC++;
		break;
	case SUB:
		R[getReg(instr, 1)] = R[getReg(instr, 2)] - R[getReg(instr, 3)];
		PC++;
		break;
	case BRZ:
		if (R[getReg(instr, 2)] == 0)
			PC += getAdressOffset(instr);
		else
			PC++;
		break;
	case JMP:
		PC = R[getReg(instr, 2)];
		break;
	case LDI:
		R[getReg(instr, 1)] = getImmediate(instr);
		PC++;
		break;
	case LD:
		R[getReg(instr, 1)] = DM.read(R[getReg(instr, 2)]);
		PC++;
		break;
	case SET:
		PC++;
		R[getReg(instr, 1)] = IM.read(PC);
		PC++;
		break;
	case ST:
		DM.write(R[getReg(instr, 1)], R[getReg(instr, 2)]);
		PC++;
		break;
	default:
		stringstream errorMsg;
		errorMsg << "Unimplemented instruction found at PC: " << PC
						 << " instr: " << hex << instr << "-- will exit";
		reportError(errorMsg.str());
		exit(-1);
	}
}
