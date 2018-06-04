#pragma once
// utils.h
#include <string>
#include "compSim.h"

std::string textMenuS(std::string question, const std::vector<std::string>& menu);
char textMenuC(std::string question, std::vector<std::string>& menu, bool compact);
char selectSimulationMode(ComputerSimulation& sim);
char selectSingleStepAction();
std::string selectProgram(std::string path);
clock_t readTime();
void reportMIPS(clock_t ticks, long long instructions);