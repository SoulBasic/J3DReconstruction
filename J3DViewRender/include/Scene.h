#ifndef _VIEWER_SCENE_H_
#define _VIEWER_SCENE_H_

#include "Window.h"

namespace VIEWER {

	class Scene
	{
	public:
		typedef TOctree<MVS::PointCloud::PointArr, MVS::PointCloud::Point::Type, 3, uint32_t, 512> OctreePoints;
		typedef TOctree<MVS::Mesh::VertexArr, MVS::Mesh::Vertex::Type, 3, uint32_t, 256> OctreeMesh;

	public:
		String name;
		String sceneName;
		MVS::Scene scene;
		Window window;
		ImageArr images; // scene photos
		ImageArr textures; // mesh textures

		OctreePoints octPoints;
		OctreeMesh octMesh;

		GLuint listPointCloud;
		GLuint listMesh;
		bool shouldClose;
		// multi-threading
		static SEACAVE::EventQueue events; // internal events queue (processed by the working threads)
		static SEACAVE::Thread thread; // worker thread

	public:
		__declspec(dllexport) Scene();
		__declspec(dllexport) ~Scene();

		void Empty();
		void Release();
		void ReleasePointCloud();
		void ReleaseMesh();
		inline bool IsValid() const { return window.IsValid(); }
		inline bool IsOpen() const { return IsValid() && !scene.IsEmpty(); }
		inline bool IsOctreeValid() const { return !octPoints.IsEmpty() || !octMesh.IsEmpty(); }

		__declspec(dllexport) bool Init(int width, int height, LPCTSTR windowName, LPCTSTR fileName = NULL, LPCTSTR meshFileName = NULL);
		bool Open(LPCTSTR fileName, LPCTSTR meshFileName = NULL);
		__declspec(dllexport) bool Export(LPCTSTR fileName, LPCTSTR exportType = NULL, bool losslessTexture = false) const;
		__declspec(dllexport) bool Export(LPCTSTR _fileName, LPCTSTR exportType, bool losslessTexture, bool b) const;
		void CompilePointCloud();
		void CompileMesh();

		void Draw();
		void ProcessEvents();
		__declspec(dllexport) void Loop();

		void CastRay(const Ray3&, int);

	protected:
		static void* ThreadWorker(void*);
	};
	/*----------------------------------------------------------------*/

} // namespace VIEWER

#endif // _VIEWER_SCENE_H_
