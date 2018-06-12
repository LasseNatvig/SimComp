#include <iostream>
#include "logger.h"
#include <time.h>

using namespace std;

LogFile::LogFile() {
	linesWritten = 0;
}

LogFile::~LogFile() {
	ofs.close();
}

bool LogFile::open(string name) {
	ofs.open(name);
	if (ofs.is_open()) {
		ofs << "Logfile " + name + " opened.\n";
		// TODO add date and time-stamp
		return true;
	}
	else {
		cerr << "Error opening file";
		return false;
	}
}

void LogFile::write(string s) {
	ofs << s;
}

void LogFile::timeStamp() {
	time_t result = time(NULL);

	#ifdef _WIN64
	char str[26];
	ctime_s(str, sizeof str, &result);
	#else
	char* str;
	str = ctime(&result);
	#endif
	ofs << str;
}

// TODO AD LINUX-code, flytt til config ?
// AD: M�tte endre til ctime_r for � f� det til � kj�re i linux
// ctime_r(&result, str);
