#pragma once
#include <qstring.h>
#include <qfile.h>
#include <Windows.h>
#include <Psapi.h>
#include "stdio.h"
#include <tlhelp32.h>
#include <qdebug.h>
#include "PlyIO.h"

#define CMD_MATCHFEATURES 101
#define CMD_SFMANDSFP 102
#define CMD_EXPORTSPARSECLOUD 103
#define CMD_EXPORTDENSECLOUD 104
#define CMD_RECONSTRUCTMESH 105
#define CMD_TEXTUREMESH 106
#define LISTIMAGES 1
#define COMPUTEFEATURES 2
#define MATCHFEATURES 3
#define SFM 4
#define SFP 5
#define COLORED 6
#define SPARSE 7
#define DENSE 8
#define DENSEFUSE 12
#define REMESH 9
#define REFINE 10
#define TEXTURE 11
#define PROCESSWORKING 0
#define PROCESSCLOSE 1
#define PROCESSERROR 2

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
	static bool tasking;
	static void connectEngine();
	static DWORD GetProcessidFromName(char* name);
	static int process;
	static int processProject;
	static int processState;
	static QString processETA;
	static bool getProcessMsg();
	static PlyIO* ply;
	static LPCTSTR charToLPCTSTR(const char* str);
};