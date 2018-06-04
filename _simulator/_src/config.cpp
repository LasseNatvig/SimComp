#include <string>
#include <vector>
#include <iostream>
#include "config.h"
#define WINDOWS
#ifndef WINDOWS
#include <dirent.h>
#endif

#ifdef WINDOWS
#include <Windows.h>
using namespace std;

vector<string> getSASMfiles(string folder) {
	// code found here:
	// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c?rq=1
	vector<string> names;
	string search_path = folder + "/*.sasm";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do { // read all (real) files in current folder
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}
#else
// code using dirent.h under UNIX?
#endif