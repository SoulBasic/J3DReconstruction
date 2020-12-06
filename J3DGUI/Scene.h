

#ifndef _VIEWER_SCENE_H_
#define _VIEWER_SCENE_H_


 // I N C L U D E S /////////////////////////////////////////////////

#include "Window.h"
#include "Global.h"

// D E F I N E S ///////////////////////////////////////////////////


// S T R U C T S ///////////////////////////////////////////////////

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
		Scene();
		~Scene();

		void Empty();
		void Release();
		void ReleasePointCloud();
		void ReleaseMesh();
		inline bool IsValid() const { return window.IsValid(); }
		inline bool IsOpen() const { return IsValid() && !scene.IsEmpty(); }
		inline bool IsOctreeValid() const { return !octPoints.IsEmpty() || !octMesh.IsEmpty(); }

		bool Init(int width, int height, LPCTSTR windowName, LPCTSTR fileName = NULL, LPCTSTR meshFileName = NULL);
		bool Open(LPCTSTR fileName, LPCTSTR meshFileName = NULL);
		bool Export(LPCTSTR fileName, LPCTSTR exportType = NULL, bool losslessTexture = false) const;
		bool Export(LPCTSTR _fileName, LPCTSTR exportType, bool losslessTexture, bool b) const;
		void CompilePointCloud();
		void CompileMesh();

		void Draw();
		void ProcessEvents();
		void Loop();

		void CastRay(const Ray3&, int);

	protected:
		static void* ThreadWorker(void*);
	};
	/*----------------------------------------------------------------*/

} // namespace VIEWER

#endif // _VIEWER_SCENE_H_
