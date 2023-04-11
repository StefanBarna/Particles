// Windows Header File
#include <Windows.h>

// C Runtime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "System.h"

#pragma comment(lib, "d2d1")

// declare additional functions for releasing interfaces, and macros for error-handling and retrieving the module's base address
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {	// inline indicates that the compiler should expand this where called -> performance purposes
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

const float dt = 0.015f; // framerate; modify to own discretion

class Engine {
public:
	Engine();
	~Engine();

	// register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// process and dispatch messages
	void RunMessageLoop();

private:
	// class members
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	// system running particle program logic
	System* m_pSys;

	// initialize device-independent resources
	HRESULT CreateDeviceIndependentResources();	// device independent resources last for the duration of the application

	// initialize device-dependent resources
	HRESULT CreateDeviceResources();			// device dependent resources are associated with a particular rendering device

	// release device-dependent resources
	void DiscardDeviceResources();

	// draw content
	HRESULT OnRender();

	// resize the render target
	void OnResize(
		UINT width,
		UINT height
	);

	// frame-by-frame process and logic
	void Update();

	// the windows procedure
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};
