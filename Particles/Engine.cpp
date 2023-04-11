#include "Engine.h"

Engine::Engine() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pSys(NULL)
{}

Engine::~Engine() {
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
}

// translates and dispatches messages
void Engine::RunMessageLoop() {
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT Engine::Initialize() {
	HRESULT hr;

	// initialize device-independent resources, such as the Direct2D factory
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr)) {
		// register the window class
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Engine::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DApp";

		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			L"D2DApp",
			L"Particles",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		if (m_hwnd) {
			// because the SetWindowPos function takes its size in pixels, we obtain the window's DPI and use it to scale the window size
			float dpi = GetDpiForWindow(m_hwnd);

			SetWindowPos(
				m_hwnd,
				NULL,
				NULL,
				NULL,
				static_cast<int>(ceil(640.f * dpi / 96.f)),
				static_cast<int>(ceil(480.f * dpi / 96.f)),
				SWP_NOMOVE
			);

			// begin timer for physics process
			SetTimer(m_hwnd, 1, static_cast<UINT>(dt * 1000), nullptr);

			// TODO: ENTRY CODE GOES HERE
			// initialize the system
			RECT clrect;
			GetClientRect(m_hwnd, &clrect);
			m_pSys = new System(
				Vector2D(clrect.left + (clrect.bottom / 10), clrect.top + (clrect.bottom / 10)),
				Vector2D(clrect.right - (clrect.right / 3), clrect.bottom - (clrect.bottom / 10)),
				1
			);

			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

// Engine entry point
int WINAPI WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
) {
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL))) {
		{
			Engine app;

			if (SUCCEEDED(app.Initialize()))
				app.RunMessageLoop();
		}
		CoUninitialize();
	}

	return 0;
}

HRESULT Engine::CreateDeviceIndependentResources() {
	HRESULT hr = S_OK;

	// create a Direct2D factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT Engine::CreateDeviceResources() {
	HRESULT hr = S_OK;

	if (!m_pRenderTarget) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// create a Direct2D render target
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);
	}

	return hr;
}

void Engine::DiscardDeviceResources() {
	SafeRelease(&m_pRenderTarget);
}

LRESULT CALLBACK Engine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	if (message == WM_CREATE) {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Engine* pApp = (Engine*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(pApp)
		);

		// TODO: WRITE ENTRY CODE HERE

		result = 1;
	}
	else {
		Engine* pApp = reinterpret_cast<Engine*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pApp) {
			switch (message) {
			case WM_TIMER:
			{
				pApp->Update();
				pApp->OnRender();
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, false);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pApp->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				// begin timer for framerate
				KillTimer(pApp->m_hwnd, 1);

				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
			result = DefWindowProc(hwnd, message, wParam, lParam);	// get windows to handle the message automatically and by default
	}

	return result;
}

HRESULT Engine::OnRender() {
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr)) {
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(0x1A2A30, 1.0f));	// TODO: select an appropriate bkgnd colour

		// TODO: ADD PROGRAM GRAPHICS HERE
		m_pSys->display(m_pRenderTarget);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

void Engine::OnResize(UINT width, UINT height) {
	if (m_pRenderTarget) {
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}

	// update window dimension information
	if (m_pSys != nullptr) {
		RECT clrect;
		GetClientRect(m_hwnd, &clrect);
		m_pSys->setbr(Vector2D(clrect.right - (clrect.right / 3), clrect.bottom - m_pSys->gettl().y()));
	}
}

void Engine::Update() {
	// TODO: WRITE PROGRAM LOGIC HERE
	m_pSys->update(dt);
	OnRender();
}