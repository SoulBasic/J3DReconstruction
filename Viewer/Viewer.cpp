#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include "Common.h"
#include "Scene.h"


using namespace VIEWER;
int main(int argc, char* argv[])
{
	auto J3DViewer = new VIEWER::Scene();

	if (!J3DViewer->Init(1361, 661, "J3D Viewer", argv[1]))
		return -1;
	J3DViewer->window.SetVisible(true);
	J3DViewer->shouldClose = false;
	J3DViewer->Loop();
	return 0;
}
/*----------------------------------------------------------------*/
