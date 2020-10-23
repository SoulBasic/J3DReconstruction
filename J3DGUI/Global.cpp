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
QString Global::textureMeshInputDir = "";
QString Global::textureMeshOutputDir = "";
QString Global::textureMeshWorkingDir = "";
QString Global::fullauto_InPutDir = "";
QString Global::fullauto_OutputDir = "";
QString Global::processETA = "";
PlyIO* Global::ply = new PlyIO("");
bool Global::tasking = false;
bool Global::autoTasking = false;
int Global::process = PROCESSWORKING;
int Global::processProject = 0;
int Global::processState = 0;
void* Global::sce = nullptr;

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
		if (strcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}


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
		//qDebug("%s  = %d ", str,buf.size());
		if (buf.size() == 21 && str.mid(0, 2) == "Es") {
			Global::processProject = DENSE;
		}
		else if (buf.size() == 21 && str.mid(0, 2) == "Pr") {
			Global::processProject = REFINE;
		}
		else if (buf.size() == 17) {
			Global::processProject = DENSEFUSE;
		}
		else if (buf.size() == 16) {
			Global::processProject = REMESH;
		}
		else if (buf.size() == 18) {
			Global::processProject = TEXTURE;
		}
		str = Processcache.readLine();
		Global::processState = str.toInt();
		str = Processcache.readLine();
		Global::processETA = str;
		Processcache.close();
		return true;
	}
	else
		return false;
}


bool Global::CheckViewerMsg()
{
	QFile Processcache("C:\\ProgramData\\J3DEngine\\ViewerCache.tmp");
	if (Processcache.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray buf = Processcache.readLine();
		QString str(buf);
		time_t tm = time(NULL);
		if (str.toLong() > 1) {
			return true;
		}

	}
	return false;


}


