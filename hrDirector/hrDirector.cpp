// hrDirector.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "joystick.h"
#include "hrDirector.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;

hrDirectorApp::hrDirectorApp() :
	m_hwnd(nullptr),
	m_pDirect2dFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pLightSlateGrayBrush(nullptr),
	m_pCornflowerBlueBrush(nullptr)
{
	m_bJoystickValid = false;
}

hrDirectorApp::~hrDirectorApp()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);

}


void hrDirectorApp::RunMessageLoop()
{	
	HACCEL hAccelTable = LoadAccelerators(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDC_HRDIRECTOR));

#ifdef NORUNTIME
	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#else
	MSG msg;

	//int loops;
	msg.message = WM_NULL;
	ULONGLONG next_game_tick = GetTickCount64();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/*
			loops = 0;
			while (GetTickCount64() > next_game_tick && loops < MAX_FRAMESKIP)
			{
			//FSX_SIM_DATA simdata;
			EnterCriticalSection(&g_pSim_Context.clientDataInterlock);
			//memcpy_s(&simdata, sizeof(FSX_SIM_DATA), (FSX_SIM_DATA*)g_pSim_Context.dataSet, sizeof(FSX_SIM_DATA));
			theInstrumentPanel->Update(g_pSim_Context.dataSet);
			LeaveCriticalSection(&g_pSim_Context.clientDataInterlock);
			//theInstrumentPanel->Update(&simdata);
			next_game_tick += SKIP_TICKS;
			loops++;
			}*/

			OnUpdate();

			OnRender();
		}
	}



	//return ExitInstance((int)msg.wParam);
#endif
	
}

HRESULT hrDirectorApp::Initialize()
{
	HRESULT hr;
	WCHAR szWindowClass[MAX_LOADSTRING];
	WCHAR szTitle[MAX_LOADSTRING];

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		LoadStringW(HINST_THISCOMPONENT, IDC_HRDIRECTOR, szWindowClass, MAX_LOADSTRING);
		LoadStringW(HINST_THISCOMPONENT, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = hrDirectorApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hIcon = LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_HRDIRECTOR));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HRDIRECTOR);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		//hInst = hInstance; // Store instance handle in our global variable

		// Create the window.
		m_hwnd = CreateWindow(
			szWindowClass, szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(1024.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(768.f * dpiY / 96.f)),
			nullptr,
			nullptr,
			HINST_THISCOMPONENT,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}






int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	hInst = hInstance;
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	if (SUCCEEDED(CoInitialize(nullptr)))
	{
		{
			hrDirectorApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
*/
LRESULT CALLBACK hrDirectorApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		hrDirectorApp *pDemoApp = (hrDirectorApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);

		result = 1;
	}
	else
	{
		hrDirectorApp *pDemoApp = reinterpret_cast<hrDirectorApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{

			case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// Parse the menu selections:
				switch (wmId)
				{
				case IDM_ABOUT:
					DialogBoxW(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, hrDirectorApp::About);
					break;
				case IDM_EXIT:
					DestroyWindow(hwnd);
					break;
				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
				}
			}
			break;
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, nullptr, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, nullptr);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

// Message handler for about box.
INT_PTR CALLBACK hrDirectorApp::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



HRESULT hrDirectorApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	
	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);


	return hr;
}


HRESULT hrDirectorApp::CreateDeviceResources()
{

	m_bJoystickValid = false;
	HRESULT hr =  m_joystick.InitDirectInput(m_hwnd);

	if (hr == S_OK)
		m_bJoystickValid = true;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		m_sizeA = 1.0f;
		m_sizeB = 1.0f;
		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red),
				&m_pLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}
	}

	return hr;
}

void hrDirectorApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void hrDirectorApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

HRESULT hrDirectorApp::OnUpdate()
{
	DIJOYSTATE2 js = { 0 };
	if (m_bJoystickValid == true)
	{
		HRESULT hs = m_joystick.UpdateInputState(js);
		if (hs == S_OK)
		{
			CString sz;
			sz.Format(_T("%d\n"), js.lX);
			OutputDebugString(sz);

			m_sizeA += (js.lX * 0.05f);
			m_sizeB += (js.lY * 0.05f);
		}
	}

	HRESULT hr = S_OK;
	
	/*m_sizeA += 0.1f;
	m_sizeB += 0.2f;
	if (m_sizeA > 50)
	{
		m_sizeA = 1.0f;
		m_sizeB = 1.0f;
	}
	*/

	return hr;
}

HRESULT hrDirectorApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		// Draw two rectangles.
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - m_sizeA,
			rtSize.height / 2 - m_sizeA,
			rtSize.width / 2 + m_sizeA,
			rtSize.height / 2 + m_sizeA
		);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - m_sizeB,
			rtSize.height / 2 - m_sizeB,
			rtSize.width / 2 + m_sizeB,
			rtSize.height / 2 + m_sizeB
		);


		m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

		// Draw the outline of a rectangle.
		m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}


