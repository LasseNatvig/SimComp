#pragma once
#include<map>
#include "memory.h"
typedef uint16_t word; 

class Memory;  // forward declaration

// Listed alphabetically, opcode values from Morris Mano 5th ed page 458
#define ADD		0b0000010
#define ADI		0b1000010
#define ADI		0b1000010
#define BRZ		0b1100000
#define HLT		0b1111111
#define JMP		0b1110000
#define LD		0b0010000
#define LDI		0b1001100
#define SET		0b1111110
#define SHL		0b0001110
#define ST		0b0100000
#define SUB		0b0000101

class Isa {
public:
	const int maxNoInstructions = 20;
	std::map<std::string, word> isaMap;
	const short noOfRegisters = 8;
	word R[8]; // Value of registers is unknown when computer boots, use LDI 0 to set a register to zero
	word PC = 0; // Always start at address zero
	Isa();
	std::string disAssembly(word instr);
	void printRegisterFile(); 
	bool printInstr(word machineInstr); // NOTE special handling of SET instruction
	short getOpCode(const word& w);
	word getRegNo1(const word& instr);
	word getRegNo2(const word& instr);
	word getRegNo3(const word& instr);
	word getImmediate(const word& instr);
	word getAdressOffset(const word& instr);
	void doInstruction(const short& opCode, const word& instr, Memory& DM, Memory& IM);
};