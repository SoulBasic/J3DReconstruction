#pragma once
#include <iostream>
#include <string>
#include <direct.h>
#include <fstream>
using namespace std;

#define CMD_MATCHFEATURES 101
#define CMD_SFMANDSFP 102
#define CMD_EXPORTSPARSECLOUD 103
#define CMD_EXPORTDENSECLOUD 104
#define CMD_RECONSTRUCTMESH 105
#define LISTIMAGES 1
#define COMPUTEFEATURES 2
#define MATCHFEATURES 3
#define SFM 4
#define SFP 5
#define COLORED 6
#define SPARSE 7
#define DENSE 8
#define REMESH 9
#define PROCESSWORKING 0
#define PROCESSCLOSE 1
#define PROCESSERROR 2



class Global
{
public:
	static int process;
	static int processProject;
	static int processState;
	static bool saveProcess();
};

