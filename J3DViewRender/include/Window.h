#ifndef _VIEWER_WINDOW_H_
#define _VIEWER_WINDOW_H_
#include "Camera.h"
#include "Image.h"

namespace VIEWER {

	class Window
	{
	public:
		GLFWwindow* window; // window handle
		CameraPtr camera; // current camera (always valid)
		Eigen::Vector2d pos, prevPos; // current and previous mouse position (normalized)

		enum SPARSE {
			SPR_POINTS = (1 << 0),
			SPR_LINES = (1 << 1),
			SPR_ALL = SPR_POINTS | SPR_LINES
		};
		SPARSE sparseType;
		unsigned minViews;
		float pointSize;
		float cameraBlend;
		bool bRenderCameras;
		bool bRenderSolid;
		bool bRenderTexture;

		enum SELECTION {
			SEL_NA = 0,
			SEL_POINT,
			SEL_TRIANGLE
		};
		SELECTION selectionType;
		Point3f selectionPoints[4];
		double selectionTimeClick, selectionTime;
		double cursorXPos, cursorYPos;
		typedef DELEGATE<bool(LPCTSTR, LPCTSTR)> ClbkOpenScene;
		ClbkOpenScene clbkOpenScene;
		typedef DELEGATE<bool(LPCTSTR, LPCTSTR, bool)> ClbkExportScene;
		ClbkExportScene clbkExportScene;
		typedef DELEGATE<void(const Ray3&, int)> ClbkRayScene;
		ClbkRayScene clbkRayScene;
		typedef DELEGATE<void(void)> ClbkCompilePointCloud;
		ClbkCompilePointCloud clbkCompilePointCloud;
		typedef DELEGATE<void(void)> ClbkCompileMesh;
		ClbkCompileMesh clbkCompileMesh;

		typedef std::unordered_map<GLFWwindow*, Window*> WindowsMap;
		static WindowsMap g_mapWindows;

	public:
		Window();
		~Window();

		void Release();
		inline bool IsValid() const { return window != NULL; }

		bool Init(int width, int height, LPCTSTR name);
		void SetCamera(CameraPtr);
		void SetName(LPCTSTR);
		__declspec(dllexport) void SetVisible(bool);
		void Reset(uint32_t minViews = 2);

		inline GLFWwindow* GetWindow() { return window; }

		void UpdateView(const ImageArr&, const MVS::ImageArr&);
		void UpdateMousePosition();

		void Resize(int width, int height);
		static void Resize(GLFWwindow* window, int width, int height);
		__declspec(dllexport) void Key(int k, int scancode, int action, int mod);
		static void Key(GLFWwindow* window, int k, int scancode, int action, int mod);
		void MouseButton(int button, int action, int mods);
		static void MouseButton(GLFWwindow* window, int button, int action, int mods);
		void Scroll(double xoffset, double yoffset);
		static void Scroll(GLFWwindow* window, double xoffset, double yoffset);
		__declspec(dllexport) void Drop(int count, const char** paths);
		static void Drop(GLFWwindow* window, int count, const char** paths);

		static void CursorPos(GLFWwindow* window, double x, double y);

	protected:
		bool IsShiftKeyPressed() const;
		bool IsCtrlKeyPressed() const;
		bool IsAltKeyPressed() const;
	};
	/*----------------------------------------------------------------*/

} // namespace VIEWER

#endif // _VIEWER_WINDOW_H_
