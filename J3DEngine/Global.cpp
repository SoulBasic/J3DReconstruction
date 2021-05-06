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


int Global::getFiles(const char* path, std::vector<std::string>& arr)
{
	int num_of_img = 0;
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	char p[700] = { 0 };
	strcpy(p, path);
	strcat(p, "\\*");
	char buf[256];
	if ((hFile = _findfirst(p, &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					continue;
			}
			else
			{

				strcpy(buf, path);
				strcat(buf, "\\");
				strcat(buf, fileinfo.name);
				struct stat st1;
				stat(buf, &st1);
				arr.push_back(buf);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return num_of_img;
}