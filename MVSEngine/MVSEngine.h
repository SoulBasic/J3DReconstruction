#pragma once

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

}
