#include "Global.h"


int Global::processProject = 0;
int Global::processState = 0;
int Global::process = PROCESSWORKING;

bool Global::saveProcess()
{

	ofstream file;
	locale loc = locale::global(locale(""));
	file.open("C:\\ProgramData\\J3DEngine\\ProcessCache.tmp", ios::out | ios::trunc);
	locale::global(loc);
	if (!file.is_open())
		return false;
	std::string project = std::to_string(Global::processProject);
	std::string state = std::to_string(Global::processState);
	std::string process = std::to_string(Global::process);
	file << process << endl;
	file << project << endl;
	file << state;
	file.close();
	return true;
}