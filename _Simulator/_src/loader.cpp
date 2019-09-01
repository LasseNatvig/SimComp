#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <assert.h>
#include "compSim.h"
#include "logger.h"
#include "memory.h"
#include "loader.h"
#include "isa.h"
#include "program.h"

using namespace std;

Loader::Loader(LogFile* logFile) : logFile(logFile) {
    lines = 0;
}

void Loader::reportError(string errorMsg) const {
    logFile->write("LOADER ERROR: " + errorMsg + " ");
    logFile->timeStamp();
    logFile->write("\n");
}

word Loader::getOperand(string opStr) const {
    /* Returns operand word */
    if (opStr[0] == 'R') {
        word op = stoi(opStr.substr(1, opStr.size() - 1));
        assert((op >= 0) && (op < 8));
        return op;
    }
    else {
        reportError("Illegal Register-operand: " + opStr + " at line " + to_string(lines));
        exit(-1);
        system("Pause"); // debug (WINDOWS SPECIFIC CODE? YES)
        return (0x00); // to avoid warning
    }
}

word Loader::getAddress(string opStr) const {
    /* Return adress as word */
    word addr = stoi(opStr);
    assert((addr >= 0) && (addr < 64));
    return addr;
}

word Loader::get3bitsConstant(string opStr) const {
    /*  */
    word op = stoi(opStr);
    assert((op >= 0) && (op < 8));
    return op;
}

void Loader::parseAlloc(Program& prog, string& name, string& type, string& size, Memory& DM) {
    /*  */
    word sizeValue;
    // Check if size is given as an already defined constant
    if (prog.intConstants.find(size) != prog.intConstants.end())
        sizeValue = prog.intConstants[size]; // size found as a constant
    else { 	// Size should be integer
        try {
            sizeValue = stoi(size); // stoi throws an exception invalid_argument if conversion cannot be done
        }
        catch (const invalid_argument& ia) {
            reportError(" at line " + to_string(lines) + ": alloc looking for size <"
                        + size + "> not found " + ia.what());
            exit(-1);
        }
    }
    if (type == "int") {
        // Allocate size no of int (words) in data memory, store address of first word in symbolTable
        if (sizeValue > 0) {
            prog.symbolTable[name] = DM.getNextFreeLocation(); // map Variable-name to address in DM
            prog.variableSizes[name] = sizeValue;
            while (--sizeValue) DM.getNextFreeLocation();
        }
    }
    else {
        cerr << "Assembler error at line " << lines; // HANDLE ERROR
        //<< ": alloc with type <" + type + "> not implemented. Skipped!" << endl;
    }  // End of alloc
}

void Loader::parseConst(Program& prog, string& name, string& type, string& value, Memory& DM) {
    if (type == "int") {
        prog.intConstants[name] = stoi(value); // Constants are "compile-time" (i.e. handled by the loader), but not stored in memory
        logFile->write("Constant named " + name + " given as " + to_string(stoi(value)) + "\n");
    }
    else if (type == "str") { // Constant string must be stored in memory
        // Will read and store the string in a "compile-time" table, and place string in allocated memory
        //cout << "**** const string under implementation \n";
        prog.strConstants[name] = value;
        // Alloc as many bytes as the length of the string + 1 for \0 termination
        word adr = DM.getNextFreeLocation();
        DM.write(adr, value[0]); // Assumes string is at least one character
        prog.symbolTable[name] = adr; // map Variable-name to address in DM
        for (unsigned int i = 1; i < value.size(); i++) {
            DM.write(DM.getNextFreeLocation(), value[i]);
        }
        DM.write(DM.getNextFreeLocation(), '\0');
    }
    else { reportError(" at line " + to_string(lines) + ": const of type <" +
                       type + "> not implemented"); exit(-1); }
}

void Loader::generateInstruction(Program& prog, string& label, string& instr, string operand[], Isa& cpu, Memory& IM) {
    if (cpu.isaMap.find(instr) == cpu.isaMap.end())
    { reportError("INSTRUCTION not found at line " + to_string(lines)); exit(-1); }
    word machineInstruction = 0;
    word opCode = cpu.isaMap[instr];
    machineInstruction = machineInstruction | (opCode << 9);
    word op1;
    if (opCode != HLT) op1 = getOperand(operand[0]);

    switch (opCode) {
    case BRZ: { // It is Ra that is given
        machineInstruction = machineInstruction | (op1 << 3);
        word op2 = getAddress(operand[1]);
        word AL = 0; word AR = 0;
        AR = 0x0007 & op2;
        AL = 0x0038 & op2;
        AL = AL << 6;
        machineInstruction = machineInstruction | AL | AR;
    }
        break;
    case LD:
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | (getOperand(operand[1]) << 3);
        break;
    case SHL:  // RD og RB
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | getOperand(operand[1]);
        break;
    case LDI:  // LDI op2 is 3-bit constant
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | get3bitsConstant(operand[1]);
        break;
    case HLT: // nothing to do
        break;
    case JMP: // Ra
        machineInstruction = machineInstruction | (op1 << 3);
        break;
    case SET: // Op2 is looked up in symbolTable below this switch
        machineInstruction = machineInstruction | (op1 << 6);
        break;
    case ST:  	// RA RB
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | (getOperand(operand[1]) << 3);
        break;
    case ADD: // ADD has 3 register operands Rd = Ra + Rb
    case SUB: // SUB has same format
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | (getOperand(operand[1]) << 3);
        machineInstruction = machineInstruction | getOperand(operand[2]);
        break;
    case ADI: // ADI has 2 register operands Rd = Ra + C
        machineInstruction = machineInstruction | (op1 << 6);
        machineInstruction = machineInstruction | (getOperand(operand[1]) << 3);
        machineInstruction = machineInstruction | get3bitsConstant(operand[2]);
        break;
    default:
        reportError("Illegal instruction at line " + to_string(lines));
        exit(-1);
        break;
    }

    word address;
    address = IM.getNextFreeLocation();
    IM.write(address, machineInstruction); // Write the instruction to next free location
    *(pcValue.end()-1) = IM.words.size()-1;
    if (opCode == SET) {
        if (prog.symbolTable.find(operand[1]) == prog.symbolTable.end()) { // NOT found as ordinary symbol (e.g. named address of variable), can be named integer constant
            if (operand[1][0] == '#') {
                word num;
                string namedConstant = operand[1].substr(1, operand[1].size() - 1);
                num = prog.intConstants[namedConstant];
                address = IM.getNextFreeLocation();
                IM.write(address, num);
            }
            else { reportError("SET instruction use name: " + operand[1] +
                        " not found in symbolTable or intConstants."); exit(-1); }
        }
        else { // it is in symbol_table
            word namedAddress = prog.symbolTable[operand[1]];
            address = IM.getNextFreeLocation();
            IM.write(address, namedAddress);
        }
    }
    if (label != "") {
        if (opCode == SET) address--; // SET instruction takes two words, label should point to first
        logFile->write("Label " + label + " was placed in " + IM.getName() + " at address: " + to_string(address) + "\n");
        prog.symbolTable[label] = address; // map name of label to address
    }
}

bool Loader::isLabel(string& s) const {
    return (s.substr(0, 1) == "_");
}

void Loader::load(string fileName, Program& prog, Isa& cpu, Memory& DM, Memory& IM) {
    ifstream asmFile;
	logFile->write("tries to open file at filename: " + fileName + "\n"); 
	//throw std::runtime_error("tries to open file at filename: " + fileName + "\n");

    asmFile.open(fileName); // TODO change to new programming style per spring 2019
    if (asmFile.fail())
		throw std::exception("Loader could not open file:");
    else
        logFile->write("Assembler file: " + fileName + "...successfully opened\n");
    pcValue.clear();
    string assemblerLine;
    while (!asmFile.eof()) {
        stringstream ss;
        getline(asmFile, assemblerLine);
        pcValue.push_back(-1);
        ss << assemblerLine;
        ++lines;
        vector<string> asm_line;
        string s;
        bool comment = false;
        while ((ss >> s) && (!comment)) { // Parsing one line of text
            if (s == ";") comment = true; // the rest of the line is a comment, just skip it
            else {
                // logg.write(s + " ");  // TODO prints every assemble-line, assign this to a DEBUG_level
                asm_line.push_back(s);
            }
        }

        string label = "";
        string operand[3];
        operand[0] = operand[1] = operand[2] = ""; // Max 3 Operands
        if (asm_line.size() > 1) { // 2 or more tokens
            if (isLabel(asm_line[0])) { // label must be first token on line, labels start with underscore
                // logg.write("label found ");  // used for debug
                label = asm_line[0];
            }
            if (asm_line[1] == "const") {
                if (asm_line.size() < 4) {
                    reportError("Error, assembler line with const requires 4 tokens");
                    exit(-1);
                }
                else
                    parseConst(prog, asm_line[0], asm_line[2], asm_line[3], DM);
            }
            else if (asm_line[1] == "alloc"){
                if (asm_line.size() < 4) {
                    reportError("Error, assembler line with alloc requires 4 tokens");
                    exit(-1);
                }
                else
                    parseAlloc(prog, asm_line[0], asm_line[2], asm_line[3], DM);
            }
            else { // 2 or more tokens, but not alloc or const
                int mnemonicIndex = 0; // index of mnemonic in asm_line vector
                if (label != "") // index 0 is label
                    mnemonicIndex = 1;
                for (unsigned int i = mnemonicIndex, j = 0; i < asm_line.size() - 1; i++, j++)
                    operand[j] = asm_line[i+1];
                for (int j = asm_line.size(); j < 3; j++)
                    operand[j] = "";
                // Not const or alloc, knows if label, knows mnemonc and ops
                generateInstruction(prog, label, asm_line[mnemonicIndex], operand, cpu, IM);
            }
        }
        else if (asm_line.size() == 1) {
            if (asm_line[0] == "HLT")
                generateInstruction(prog, label, asm_line[0], operand, cpu, IM);
            else {
                reportError("Error, assembler line with 1 token invalid, only HLT accepted");
                exit(-1);
            }
        }
        // zero token is OK, means line with only comments.
    }
    prog.valid = true;
}
