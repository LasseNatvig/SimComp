#pragma once
#include <map>
#include "memory.h"
#include "logger.h"

typedef uint16_t word;

class Memory;  // Forward declaration

/* Listed alphabetically, opcode values from Morris Mano 5th ed page 458 */
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
private:
	const int maxNoInstructions = 20;
	const short noOfRegisters = 8;
	word R[8]; // Value of registers is unknown when computer boots, use LDI 0 to set a register to zero
	LogFile* logFile;

public:
	word PC = 0; // Always start at address zero
	std::map<std::string, word> isaMap; // Maps instruction string with binary opcode

	/* Constructor(s) */
	Isa(LogFile* logFile);

	/* Core functionality */
	std::string disAssembly(word instr); // Returns string representation of instruction if found
	void doInstruction(const short& opCode, const word& instr, Memory& DM, Memory& IM); // Excutes instruction
	void reportError(std::string errorMsg) const;

	/* Get functions */
  std::string getRegisterFile() const;
  std::string getRegister(int registerNum) const;
  std::string getInstr(word machineInstr) const;
	word getOpCode(const word& w) const;
	word getReg(const word& instr, unsigned int regNo) const;
	word getImmediate(const word& instr) const;
	word getAdressOffset(const word& instr) const;
	const int getMaxNoInstructions() const { return maxNoInstructions; }
};
