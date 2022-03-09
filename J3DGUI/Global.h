#pragma once
#include <qstring.h>
#include <qfile.h>
#include <Windows.h>
#include <Psapi.h>
#include "stdio.h"
#include <tlhelp32.h>
#include <qdebug.h>
#include <time.h>
#include <io.h>

//#define SUPPORT_OSG //支持打开和转换OSG文件，需要存在osgviewer.exe、osgconv.exe和相关osg库dll文件
//#define SUPPORT_GLTF //支持GLTF输出，需安装obj2gltf插件，用于cesium显示

#define CMD_FULLAUTO 100
#define CMD_MATCHFEATURES 101
#define CMD_SFMANDSFP 102
#define CMD_EXPORTSPARSECLOUD 103
#define CMD_EXPORTDENSECLOUD 104
#define CMD_RECONSTRUCTMESH 105
#define CMD_TEXTUREMESH 106
#define CMD_IMPORTFROMBE 107
#define LISTIMAGES 1
#define COMPUTEFEATURES 2
#define MATCHFEATURES 3
#define SFM 4
#define SFP 5
#define COLORED 6
#define SPARSECLOUD 7
#define DENSE 8
#define DENSEFUSE 12
#define REMESH 9
#define REFINE 10
#define TEXTURE 11
#define FULLAUTO 13
#define PROCESSWORKING 0
#define PROCESSCLOSE 1
#define PROCESSERROR 2
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define KEYMSG_T 2320
#define KEYMSG_M 2321
#define KEYMSG_C 2322
#define KEYMSG_P 2323
#define KEYMSG_UP 2324
#define KEYMSG_DOWN 2325
#define KEYMSG_ESC 2326
#define KEYMSG_UPSHIFT 2327
#define KEYMSG_DOWNSHIFT 2328

class Global
{
public:
	static int engineTid;
	static QString imagesInfoFile;
	static QString imagesDir;
	static QString matchesOutputDir;
	static QString matchesDir;
	static QString sfmOutputDir;
	static QString densifyInputDir;
	static QString densifyOutputDir;
	static QString densifyWorkingDir;
	static QString reconstructMeshInputDir;
	static QString reconstructMeshOutputDir;
	static QString reconstructMeshWorkingDir;
	static QString textureMeshInputDir;
	static QString textureMeshOutputDir;
	static QString textureMeshWorkingDir;
	static QString fullauto_InPutDir;
	static QString fullauto_OutputDir;
	static QString importWorkingDir;
	static bool tasking;
	static bool autoTasking;
	static void connectEngine();
	static DWORD GetProcessidFromName(const char* name);
	static int process;
	static int processProject;
	static int processState;
	static QString processETA;
	static bool getProcessMsg();
	static bool intersecting;
	static int getFiles(const char* path, std::vector<std::string>& arr, bool fullName = false);
};