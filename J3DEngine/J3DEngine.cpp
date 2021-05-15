#define _USE_MATH_DEFINES
#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include "PresetConfigureImages.hpp"
#include "MatchImages.hpp"
#include "StructureFromMotion.hpp"
#include "StructureFromPoses.hpp"
#include "ExportSparseCloud.hpp"
#include "OriginPoints.hpp"
#include "BlocksExchangeInterface.hpp"
#include <algorithm>
#include <direct.h>
#include <fstream>
#include "Global.h"
#include "MVSEngine.h"
#include "third_party/cmdLine/cmdLine.h"


HWND m_hWnd;
int STATE_RETURN;


bool saveTid()
{
	int tid = GetCurrentThreadId();
	ofstream file;
	locale loc = locale::global(locale(""));
	file.open("C:\\ProgramData\\J3DEngine\\ProgramCache.tmp", ios::out | ios::trunc);
	locale::global(loc);
	if (!file.is_open())
		return false;
	file << std::to_string(tid) << endl;
	file.close();
	return true;
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	m_hWnd = FindWindow("Qt5QWindowIcon", "J3D可视化平台");
	if (!saveTid() || !Global::saveProcess())
		printf("写入缓存失败，请检查软件权限，或使用管理员身份运行 \n");
}
void MsgProc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (!m_hWnd) {
		return;
	}

	switch (msg)
	{
	case CMD_MATCHFEATURES:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string imagesInputDir;
		std::string sensorWidthDataBaseDir = "./SenWidDB.txt";
		std::string matchesOutputDir;
		std::string EigenMatrix;// EigenMatrixFormat"f;0;ppx;0;f;ppy;0;0;1"
		std::string describerMethod;
		std::string featureQuality;
		bool upRight;
		bool forceCompute;
		std::string geometricModel;
		float distanceRatio;
		bool forceMatch;

		printf("\n任务呼叫：MATCHFEATURES \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		std::string temp;
		getline(cmdCache, temp);
		if (temp != "matchfeature")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, imagesInputDir);
		getline(cmdCache, matchesOutputDir);
		getline(cmdCache, temp);
		EigenMatrix = temp;
		if (EigenMatrix == "NULL")
		{
			EigenMatrix = "";
		}
		getline(cmdCache, describerMethod);
		getline(cmdCache, featureQuality);
		getline(cmdCache, temp);
		if (temp == "0")
			upRight = false;
		else
			upRight = true;
		getline(cmdCache, temp);
		if (temp == "0")
			forceCompute = false;
		else
			forceCompute = true;
		getline(cmdCache, geometricModel);
		getline(cmdCache, temp);
		distanceRatio = atof(temp.c_str());
		getline(cmdCache, temp);
		if (temp == "0")
			forceMatch = false;
		else
			forceMatch = true;

		cmdCache.close();

		STATE_RETURN = LoadingImages(imagesInputDir, matchesOutputDir, sensorWidthDataBaseDir, EigenMatrix, "1.0;1.0;1.0");
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("加载图片失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("加载图片成功\n");
		STATE_RETURN = GetFeatures(
			matchesOutputDir + "/sfm_data.json",
			matchesOutputDir,
			describerMethod,
			"",
			upRight,
			forceCompute);

		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("获取特征信息失败\n");
			Global::process = PROCESSERROR;
			break;

		}
		printf("获取特征点完成，准备开始匹配特征点\n");

		STATE_RETURN = GetMatches(
			matchesOutputDir + "/sfm_data.json",
			matchesOutputDir,
			geometricModel,
			"",
			"AUTO",
			-1,
			2048,
			0U,
			distanceRatio, forceMatch);

		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("匹配特征信息失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("匹配完成\n");
		printf("\n任务完成\n");
		Global::process = PROCESSCLOSE;
		break;
	}

	case CMD_SFMANDSFP:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string matchesDir, sfmOutputDir;
		int triangulationMethod, resectionMethod;
		printf("\n任务呼叫：SFMANDSFP \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		std::string temp;
		getline(cmdCache, temp);
		if (temp != "sfmandsfp")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, matchesDir);
		getline(cmdCache, sfmOutputDir);
		getline(cmdCache, temp);
		triangulationMethod = atoi(temp.c_str());
		getline(cmdCache, temp);
		resectionMethod = atoi(temp.c_str());
		cmdCache.close();
		STATE_RETURN = StructureFromMotion
		(
			matchesDir + "\\sfm_data.json",
			matchesDir,
			"",
			sfmOutputDir,
			"",
			"",
			"ADJUST_ALL",
			3,
			true,
			true,
			triangulationMethod,
			resectionMethod);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM重建失败\n");
			Global::process = PROCESSERROR;
			break;

		}

		printf("进行SFM数据精细化\n");
		STATE_RETURN = ConvertCoorsToOrigin
		(
			sfmOutputDir + "/sfm_data.bin",
			sfmOutputDir
		);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM数据精细化失败\n");
			Global::process = PROCESSERROR;
			break;

		}

		printf("进行点云上色\n");
		STATE_RETURN = PrintPointColors(sfmOutputDir + "/sfm_data_local.bin", sfmOutputDir + "/colored.ply");
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM点云上色失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("进行SFP重构\n");
		STATE_RETURN = StructureFromPoses(
			sfmOutputDir + "/sfm_data_local.bin",
			matchesDir,
			sfmOutputDir + "/robust.bin",
			matchesDir + "/matches.f.bin");
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFP重构失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("SFP重构成功\n");
		printf("进行稀疏点云输出\n");
		STATE_RETURN = ExportSparseCloud
		(
			sfmOutputDir + "/robust.bin",
			sfmOutputDir + "/SparseCloud.J3D",
			sfmOutputDir + "/undistorted_images",
			sfmOutputDir
		);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("输出稀疏点云失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("\n任务完成\n");
		Global::process = PROCESSCLOSE;
		break;
	}

	case CMD_EXPORTDENSECLOUD:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string densifyInputDir, densifyOutputDir, densifyWorkingDir;
		printf("\n任务呼叫：DENSIFYPOINTCLOUD \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		std::string temp;
		getline(cmdCache, temp);
		if (temp != "densifypointcloud")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, densifyInputDir);
		getline(cmdCache, densifyWorkingDir);
		getline(cmdCache, densifyOutputDir);
		cmdCache.close();
		char* cmd[7];
		char t[200];
		GetModuleFileNameA(NULL, t, 200);
		cmd[0] = t;
		cmd[1] = "-i";
		cmd[2] = (char*)densifyInputDir.data();
		cmd[3] = "-w";
		cmd[4] = (char*)densifyWorkingDir.data();
		cmd[5] = "-o";
		cmd[6] = (char*)densifyOutputDir.data();
		Global::process = !MVSEngine::DensifyPointCloud(7, cmd);
		break;
	}

	case CMD_RECONSTRUCTMESH:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string reconstructMeshInputDir, reconstructMeshOutputDir, reconstructMeshWorkingDir;
		printf("\n任务呼叫：RECONSTRUCTMESH \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		std::string temp;
		getline(cmdCache, temp);
		if (temp != "reconstructmesh")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		getline(cmdCache, reconstructMeshInputDir);
		getline(cmdCache, reconstructMeshWorkingDir);
		getline(cmdCache, reconstructMeshOutputDir);
		cmdCache.close();
		char* cmd[9];
		char t[200];
		GetModuleFileNameA(NULL, t, 200);
		cmd[0] = t;
		cmd[1] = "-i";
		cmd[2] = (char*)reconstructMeshInputDir.data();
		cmd[3] = "-d";
		cmd[4] = "4";
		cmd[5] = "-o";
		cmd[6] = (char*)reconstructMeshOutputDir.data();
		cmd[7] = "-w";
		cmd[8] = (char*)reconstructMeshWorkingDir.data();
		int status = MVSEngine::ReconstructMesh(9, cmd);
		if (STATE_RETURN != EXIT_SUCCESS) {
			Global::process = PROCESSWORKING;
			printf("任务失败，请检查路径和文件是否正确\n");
			break;
		}
		cmd[0] = t;
		cmd[1] = "-i";
		cmd[2] = (char*)reconstructMeshOutputDir.data();
		cmd[3] = "--resolution-level";
		cmd[4] = "2";
		cmd[5] = "-o";
		cmd[6] = (char*)reconstructMeshOutputDir.data();
		cmd[7] = "-w";
		cmd[8] = (char*)reconstructMeshWorkingDir.data();
		Global::process = !MVSEngine::RefineMesh(9, cmd);
		break;
	}

	case CMD_TEXTUREMESH:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string textureMeshInputDir, textureMeshOutputDir, textureMeshWorkingDir, exportFormat;
		printf("\n任务呼叫：TEXTUREMESH \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		std::string temp;
		getline(cmdCache, temp);
		if (temp != "texturemesh")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		getline(cmdCache, textureMeshInputDir);
		getline(cmdCache, textureMeshWorkingDir);
		getline(cmdCache, textureMeshOutputDir);
		getline(cmdCache, exportFormat);
		bool isOsgb = false;
		if ("osgb" == exportFormat)
		{
			exportFormat = "obj";
			isOsgb = true;
		}
		cmdCache.close();
		char* cmd[9];
		char t[200];
		GetModuleFileNameA(NULL, t, 200);
		cmd[0] = t;
		cmd[1] = "-i";
		cmd[2] = (char*)textureMeshInputDir.data();
		cmd[3] = "-o";
		cmd[4] = (char*)textureMeshOutputDir.data();
		cmd[5] = "-w";
		cmd[6] = (char*)textureMeshWorkingDir.data();
		cmd[7] = "--export-type";
		cmd[8] = (char*)exportFormat.data();
		int status = MVSEngine::TextureMesh(9, cmd);

		if (status == EXIT_SUCCESS)
		{
			if (isOsgb)
			{
				string cmdt = "osgcv.exe " + textureMeshWorkingDir + "/TEXTURE_Mesh.obj " + textureMeshWorkingDir + "/TEXTURE_Mesh.osgb";
				::system(cmdt.c_str());
			}
			Global::process = PROCESSCLOSE;
		}
		else
		{
			Global::process = PROCESSWORKING;
		}
		break;

	}

	case CMD_FULLAUTO:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();


		std::string imagesInputDir;
		std::string sensorWidthDataBaseDir = "./SenWidDB.txt";
		std::string matchesOutputDir;
		std::string EigenMatrix;// EigenMatrixFormat"f;0;ppx;0;f;ppy;0;0;1"
		std::string describerMethod;
		std::string featureQuality;
		bool upRight;
		bool forceCompute;
		std::string geometricModel;
		float distanceRatio;
		bool forceMatch;

		std::string matchesDir, sfmOutputDir;
		int triangulationMethod, resectionMethod;

		std::string densifyInputDir, densifyOutputDir, densifyWorkingDir;

		std::string reconstructMeshInputDir, reconstructMeshOutputDir, reconstructMeshWorkingDir;

		std::string textureMeshInputDir, textureMeshOutputDir, textureMeshWorkingDir, exportFormat;

		printf("\n任务呼叫：FULLAUTO \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\fullautoCmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		std::string temp;
		getline(cmdCache, temp);
		if (temp != "matchfeature")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, imagesInputDir);
		getline(cmdCache, matchesOutputDir);
		getline(cmdCache, EigenMatrix);
		if (EigenMatrix == "NULL")
		{
			EigenMatrix = "";
		}
		getline(cmdCache, describerMethod);
		getline(cmdCache, featureQuality);
		getline(cmdCache, temp);
		if (temp == "0")
			upRight = false;
		else
			upRight = true;
		getline(cmdCache, temp);
		if (temp == "0")
			forceCompute = false;
		else
			forceCompute = true;
		getline(cmdCache, geometricModel);
		getline(cmdCache, temp);
		distanceRatio = atof(temp.c_str());
		getline(cmdCache, temp);
		if (temp == "0")
			forceMatch = false;
		else
			forceMatch = true;
		getline(cmdCache, temp);
		if (temp != "sfmandsfp")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, matchesDir);
		getline(cmdCache, sfmOutputDir);
		getline(cmdCache, temp);
		triangulationMethod = atoi(temp.c_str());
		getline(cmdCache, temp);
		resectionMethod = atoi(temp.c_str());
		getline(cmdCache, temp);
		if (temp != "densifypointcloud")
		{
			printf("任务失败,无法获取任务参数\n");

			Global::process = PROCESSERROR;
			break;
		}

		getline(cmdCache, densifyInputDir);
		getline(cmdCache, densifyWorkingDir);
		getline(cmdCache, densifyOutputDir);

		getline(cmdCache, temp);
		if (temp != "reconstructmesh")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		getline(cmdCache, reconstructMeshInputDir);
		getline(cmdCache, reconstructMeshWorkingDir);
		getline(cmdCache, reconstructMeshOutputDir);
		getline(cmdCache, temp);
		if (temp != "texturemesh")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		getline(cmdCache, textureMeshInputDir);
		getline(cmdCache, textureMeshWorkingDir);
		getline(cmdCache, textureMeshOutputDir);
		getline(cmdCache, exportFormat);
		cmdCache.close();
		STATE_RETURN = LoadingImages(imagesInputDir, matchesOutputDir, sensorWidthDataBaseDir, EigenMatrix, "1.0;1.0;1.0");
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("加载图片失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("加载图片成功\n");
		STATE_RETURN = GetFeatures(
			matchesOutputDir + "/sfm_data.json",
			matchesOutputDir,
			describerMethod,
			"",
			upRight,
			forceCompute);
		Sleep(3000);

		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("获取特征信息失败\n");
			Global::process = PROCESSERROR;
			break;

		}
		printf("获取特征点完成，准备开始匹配特征点\n");
		STATE_RETURN = GetMatches(
			matchesOutputDir + "/sfm_data.json",
			matchesOutputDir,
			geometricModel,
			"",
			"AUTO",
			-1,
			2048,
			0U,
			distanceRatio, forceMatch);
		Sleep(3000);

		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("匹配特征信息失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("匹配完成\n");
		STATE_RETURN = StructureFromMotion(
			matchesDir + "\\sfm_data.json",
			matchesDir,
			"",
			sfmOutputDir,
			"",
			"",
			"ADJUST_ALL",
			3,
			true,
			true,
			triangulationMethod,
			resectionMethod);
		Sleep(3000);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM重建失败\n");
			Global::process = PROCESSERROR;
			break;

		}

		printf("进行SFM数据精细化\n");
		STATE_RETURN = ConvertCoorsToOrigin
		(
			sfmOutputDir + "/sfm_data.bin",
			sfmOutputDir
		);
		Sleep(2000);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM数据精细化失败\n");
			Global::process = PROCESSERROR;
			break;

		}

		printf("进行点云上色\n");
		STATE_RETURN = PrintPointColors(sfmOutputDir + "/sfm_data_local.bin", sfmOutputDir + "/colored.ply");
		Sleep(3000);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM点云上色失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("进行SFP重构\n");
		STATE_RETURN = StructureFromPoses(
			sfmOutputDir + "/sfm_data_local.bin",
			matchesDir,
			sfmOutputDir + "/robust.bin",
			matchesDir + "/matches.f.bin");
		Sleep(3000);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFP重构失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("SFP重构成功\n");
		printf("进行稀疏点云输出\n");
		STATE_RETURN = ExportSparseCloud
		(
			sfmOutputDir + "/robust.bin",
			sfmOutputDir + "/SparseCloud.J3D",
			sfmOutputDir + "/undistorted_images",
			sfmOutputDir
		);
		Sleep(3000);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("输出稀疏点云失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		char* cmd[7];
		char t[200];
		GetModuleFileNameA(NULL, t, 200);
		cmd[0] = t;
		cmd[1] = "-i";
		cmd[2] = (char*)densifyInputDir.data();
		cmd[3] = "-w";
		cmd[4] = (char*)densifyWorkingDir.data();
		cmd[5] = "-o";
		cmd[6] = (char*)densifyOutputDir.data();
		STATE_RETURN = MVSEngine::DensifyPointCloud(7, cmd);
		if (STATE_RETURN != EXIT_SUCCESS) {
			Global::process = PROCESSWORKING;
			printf("任务失败，请检查路径和文件是否正确\n");
			break;
		}
		Sleep(5000);
		char* cmd1[9];
		char t1[200];
		GetModuleFileNameA(NULL, t1, 200);
		cmd1[0] = t1;
		cmd1[1] = "-i";
		cmd1[2] = (char*)reconstructMeshInputDir.data();
		cmd1[3] = "-d";
		cmd1[4] = "4";
		cmd1[5] = "-o";
		cmd1[6] = (char*)reconstructMeshOutputDir.data();
		cmd1[7] = "-w";
		cmd1[8] = (char*)reconstructMeshWorkingDir.data();
		STATE_RETURN = MVSEngine::ReconstructMesh(9, cmd1);
		if (STATE_RETURN != EXIT_SUCCESS) {
			Global::process = PROCESSWORKING;
			printf("任务失败，请检查路径和文件是否正确\n");
			break;
		}
		Sleep(5000);
		cmd1[0] = t1;
		cmd1[1] = "-i";
		cmd1[2] = (char*)reconstructMeshOutputDir.data();
		cmd1[3] = "--resolution-level";
		cmd1[4] = "2";
		cmd1[5] = "-o";
		cmd1[6] = (char*)reconstructMeshOutputDir.data();
		cmd1[7] = "-w";
		cmd1[8] = (char*)reconstructMeshWorkingDir.data();
		STATE_RETURN = MVSEngine::RefineMesh(9, cmd1);
		if (STATE_RETURN != EXIT_SUCCESS) {
			Global::process = PROCESSWORKING;
			printf("任务失败，请检查路径和文件是否正确\n");
			break;
		}
		Sleep(5000);
		bool isOsgb = false;
		if ("osgb" == exportFormat)
		{
			exportFormat = "obj";
			isOsgb = true;
		}
		char* cmd2[9];
		char t2[200];
		GetModuleFileNameA(NULL, t2, 200);
		cmd2[0] = t2;
		cmd2[1] = "-i";
		cmd2[2] = (char*)textureMeshInputDir.data();
		cmd2[3] = "-o";
		cmd2[4] = (char*)textureMeshOutputDir.data();
		cmd2[5] = "-w";
		cmd2[6] = (char*)textureMeshWorkingDir.data();
		cmd2[7] = "--export-type";
		cmd2[8] = (char*)exportFormat.data();
		STATE_RETURN = MVSEngine::TextureMesh(9, cmd2);
		if (STATE_RETURN != EXIT_SUCCESS) {
			Global::process = PROCESSWORKING;
			printf("任务失败，请检查路径和文件是否正确\n");
			break;
		}
		if (isOsgb)
		{
			string cmdt = "osgcv.exe " + textureMeshWorkingDir + "/TEXTURE_Mesh.obj " + textureMeshWorkingDir + "/TEXTURE_Mesh.osgb";
			::system(cmdt.c_str());
		}
		Global::process = PROCESSCLOSE;
		break;
	}

	case CMD_IMPORTFROMBE:
	{
		Global::process = PROCESSWORKING;
		Global::saveProcess();
		std::string inputFileName, workDir;
		printf("\n任务呼叫：IMPORTFROMBE \n\n");
		ifstream cmdCache;
		cmdCache.open(("C:\\ProgramData\\J3DEngine\\cmdCache.tmp"), ios::in | ios::_Nocreate);
		if (!cmdCache)
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		std::string temp;
		getline(cmdCache, temp);
		if (temp != "importfromblocksexchange")
		{
			printf("任务失败,无法获取任务参数\n");
			Global::process = PROCESSERROR;
			break;
		}
		getline(cmdCache, inputFileName);
		getline(cmdCache, workDir);
		int STATE_RETURN = BlocksExchange2openMVG(inputFileName, workDir);
		if (STATE_RETURN == false)
		{
			printf("导入Blocks Exchange文件失败!\n");
			Global::process = PROCESSERROR;
			break;
		}
		Sleep(1000);
		printf("进行SFM坐标数据精细化\n");
		STATE_RETURN = ConvertCoorsToOrigin
		(
			workDir + "/sfm_data.bin",
			workDir
		);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("SFM坐标数据精细化失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("进行稀疏点云输出\n");
		STATE_RETURN = ExportSparseCloud
		(
			workDir + "/sfm_data_local.bin",
			workDir + "/SparseCloud.J3D",
			workDir + "/undistorted_images",
			workDir
		);
		if (STATE_RETURN == EXIT_FAILURE)
		{
			printf("输出稀疏点云失败\n");
			Global::process = PROCESSERROR;
			break;
		}
		printf("成功输出到文件SparseCloud.J3D\n");
		printf("导入数据完成，之后可以继续使用J3D程序进行模型重建或坐标映射工作\n");
		printf("\n任务完成\n");
		Global::process = PROCESSCLOSE;
		break;
	}
	}
	return;
}
int main()
{
	printf("\n正在初始化引擎，请稍等\n");
	MSG msg;
	SetTimer(NULL, 0, 1000, NULL);
	_mkdir("C:\\ProgramData\\J3DEngine");
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	mode &= ~ENABLE_INSERT_MODE;
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, mode);
	SetConsoleTitle("J3DEngine");
	int tid = GetCurrentThreadId();
	if (!saveTid())
	{
		MessageBoxA(nullptr, "写配置文件失败，请检查软件权限并使用管理员身份运行 ", "权限错误 ", MB_OK);
		return -1;
	}
	m_hWnd = FindWindow("Qt5QWindowIcon", "J3D可视化平台");
	if (m_hWnd) {
		printf("找到J3DGUI窗口");
		SendMessage(m_hWnd, WM_USER, tid, 0);
	}
	::system("cls");
	printf("\n-----------------------------------\n");
	printf("        欢迎使用J3DEngine V2.0        \n");
	printf("            程序初始化成功             \n");
	printf("        请使用J3DGUI程序发起指令       \n");
	printf("     @Basic All rights reserved    \n");
	printf("-----------------------------------\n");
	Global::processProject = 0;
	Global::processState = 0;
	Global::process = PROCESSWORKING;

	while (int a = GetMessage(&msg, NULL, 0, 0))
	{
		if (a != -1)
		{
			if (msg.message == WM_TIMER)
				TimerProc(NULL, 0, 0, 0);
			MsgProc(msg.message, msg.wParam, msg.lParam);
		}

	}

	return 0;
}
