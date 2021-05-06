#pragma once
#include <vector>
#include <io.h>
namespace MVSEngine
{
	bool Initialize_Dense(size_t argc, LPCTSTR* argv);
	void Finalize_Dense();
	__declspec(dllexport) int DensifyPointCloud(int num, char* cmd[]);

	bool Initialize_ReconstructMesh(size_t argc, LPCTSTR* argv);
	void Finalize_ReconstructMesh();
	__declspec(dllexport) int ReconstructMesh(int num, char* cmd[]);

	bool Initialize_RefineMesh(size_t argc, LPCTSTR* argv);
	void Finalize_RefineMesh();
	__declspec(dllexport) int RefineMesh(int num, char* cmd[]);

	bool Initialize_TextureMesh(size_t argc, LPCTSTR* argv);
	void Finalize_TextureMesh();
	__declspec(dllexport) int TextureMesh(int num, char* cmd[]);

	int getFiles(const char* path, std::vector<std::string>& arr)
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
}
