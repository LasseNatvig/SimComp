#pragma once
#include <string>
#include <fstream>

class LogFile {
private:
	long linesWritten; // Line count
	std::ofstream ofs;
public:
	LogFile();
	~LogFile();
	bool open(std::string name);
	void write(std::string s); // Write to log
	void timeStamp(); // Write time stamp to file
};
