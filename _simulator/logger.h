#pragma once
#include <string>
#include <fstream>

class LogFile {
private:
	long linesWritten;
	std::ofstream ofs;
public:
	LogFile();
	~LogFile();
	bool open(std::string name);
	void write(std::string s);
	void timeStamp();
};