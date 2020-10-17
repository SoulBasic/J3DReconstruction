#include "my_hook.h"
void* MyHook::window = nullptr;
int MyHook::Messsages(){
	while (msg.message != WM_QUIT){ //while we do not close our application
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	UninstallHook(); //if we close, let's uninstall our hook
	return (int)msg.wParam; //return the messages
}
void MyHook::InstallHook(void* window){
	/*
	SetWindowHookEx(
	WM_MOUSE_LL = mouse low level hook type,
	MyMouseCallback = our callback function that will deal with system messages about mouse
	NULL, 0);

	c++ note: we can check the return SetWindowsHookEx like this because:
	If it return NULL, a NULL value is 0 and 0 is false.
	*/
	MyHook::window = window;
	if (!(keyboardhook == SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyBoardCallback, NULL, 0)))
	{
		//printf_s("Error: %x \n", GetLastError());
	}
}

void MyHook::UninstallHook(){
	/*
	uninstall our hook using the hook handle
	*/
	UnhookWindowsHookEx(hook);
	UnhookWindowsHookEx(keyboardhook);
}


LRESULT WINAPI MyKeyBoardCallback( int nCode, WPARAM wParam, LPARAM lParam )
{
	KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

	if (nCode == 0)
	{
		

		switch(wParam)
		{
		case WM_KEYUP:
		{
			if (!pKeyStruct)
			{
				break;
			}

			//switch (pKeyStruct->vkCode)
			//{
			//case 84://T 
			//{
			//	MyHook::window->Key(GLFW_KEY_T, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 77://M
			//{
			//	MyHook::window->Key(GLFW_KEY_M, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 67://C
			//{
			//	MyHook::window->Key(GLFW_KEY_C, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 80://P
			//{
			//	MyHook::window->Key(GLFW_KEY_P, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 265://up
			//{
			//	MyHook::window->Key(GLFW_KEY_UP, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 264://down
			//{
			//	MyHook::window->Key(GLFW_KEY_DOWN, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//case 256://esc
			//{
			//	MyHook::window->Key(GLFW_KEY_ESCAPE, 0, GLFW_RELEASE, GLFW_MOD_SUPER);
			//	break;
			//}
			//}
					
		}break;
		
		
		
		case WM_SYSKEYDOWN:{
			printf_s("Not Sys Key\n");
						   }break;
		}

		
	}
	return CallNextHookEx(MyHook::Instance().keyboardhook, nCode, wParam, lParam);
}