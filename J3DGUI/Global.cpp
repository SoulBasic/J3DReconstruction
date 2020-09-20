#include "Global.h"


int Global::engineTid = 0;
QString Global::imagesDir = "";
QString Global::imagesInfoFile = "";
QString Global::matchesOutputDir = "";
QString Global::matchesDir = "";
QString Global::sfmOutputDir = "";
QString Global::densifyInputDir = "";
QString Global::densifyOutputDir = "";
QString Global::densifyWorkingDir = "";
QString Global::reconstructMeshInputDir = "";
QString Global::reconstructMeshOutputDir = "";
QString Global::reconstructMeshWorkingDir = "";
bool Global::tasking = false;
void Global::connectEngine()
{
	QFile file("C:\\ProgramData\\J3DEngine\\ProgramCache.tmp");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QString tid = QString(file.readAll());
	file.close();
	Global::engineTid = tid.toInt();
	if (tid == 0)
	{
		return;
	}
	return;
}
DWORD Global::GetProcessidFromName(char* name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;

		wchar_t * wtext = pe.szExeFile;
		DWORD dwNmu = WideCharToMultiByte(CP_OEMCP, NULL, wtext, -1, NULL, 0, NULL, FALSE);
		char * psTest;
		psTest = new char[dwNmu];
		WideCharToMultiByte(CP_OEMCP, NULL, wtext, -1, psTest, dwNmu, NULL, FALSE);
		std::string a = psTest;
		delete[]psTest;

		if (strcmp(a.c_str(), name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}

int Global::process = PROCESSWORKING;
int Global::processProject = 0;
int Global::processState = 0;
bool Global::getProcessMsg()
{
	QFile Processcache("C:\\ProgramData\\J3DEngine\\ProcessCache.tmp");
	if (Processcache.open(QIODevice::ReadOnly | QIODevice::Text))
	{

		QByteArray buf = Processcache.readLine();
		QString str(buf);
		Global::process = str.toInt();
		buf = Processcache.readLine();
		str = buf;
		qDebug("%d", buf.size());
		if (buf.size() == 21)
			str = "8";
		Global::processProject = str.toInt();
		buf = Processcache.readLine();
		str = QString(buf);
		Global::processState = str.toInt();
		Processcache.close();
		return true;
	}
	else
		return false;
}
