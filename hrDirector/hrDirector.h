#pragma once

#include "resource.h"
class CJoystickDevice;

#define JOYSTICK_SMOOTH_SIZE 5

class hrDirectorApp
{
public:
	static const float GraticuleCase;
	static const float GraticuleRadius;
public:
	hrDirectorApp();
	~hrDirectorApp();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();
	float m_x;
	float m_y;
	double radian;
	double halfradian;
	// Draw content.
	HRESULT OnRender();
	HRESULT OnUpdate();

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

protected:
	HRESULT CreateBars(ID2D1Factory* pDirect2dFactoryh, float x, float y);
	HRESULT CreateResources(ID2D1Factory* pDirect2dFactory, ID2D1HwndRenderTarget* pRenderTarget);
	void DiscardResources();
	void Update(DIJOYSTATE2 js);
	void Render(D2D1_SIZE_F size, ID2D1HwndRenderTarget* pRenderTarget);
protected:
	void RenderGrid(D2D1_SIZE_F rtSize, ID2D1HwndRenderTarget* pRenderTarget);

protected:
	ID2D1PathGeometry* m_pGraticule;
	ID2D1PathGeometry* m_pBars;
	HRESULT CreateGraticule(ID2D1Factory* pDirect2dFactory, float length);
protected:


	void RotateLine(float& x1, float& y1, float& x2, float& y2, double deg);
private:
	HWND m_hwnd;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pDirect2dFactory;
	
	IDWriteFactory* m_pDWriteFactory;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	ID2D1SolidColorBrush* m_pDimGrayBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pYellowBrush;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pRedBrush;

	void SmoothJoystick(DIJOYSTATE2 js, POINTF& ps);
	POINT m_joystickpt[JOYSTICK_SMOOTH_SIZE];
	CJoystickDevice m_joystick;
	bool m_bJoystickValid;
	bool m_bIsGameRunning;
	int m_bFlag;
};
