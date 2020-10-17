
#include <Windows.h>
#include <iostream>
#include "Scene.h"

#define GLFW_KEY_C                  67
#define GLFW_KEY_M                  77
#define GLFW_KEY_P                  80
#define GLFW_KEY_T                  84
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
class MyHook{
public:
	//single ton
	static MyHook& Instance(){
		static MyHook myHook;
		return myHook;
	}	
	HHOOK hook; // handle to the hook	
	static void* window;
	void InstallHook(void* window); // function to install our hook
	void UninstallHook(); // function to uninstall our hook

	MSG msg; // struct with information about all messages in our queue
	int Messsages(); // function to "deal" with our messages 

	HHOOK keyboardhook;
};
LRESULT WINAPI MyKeyBoardCallback(int nCode, WPARAM wParam, LPARAM lParam);