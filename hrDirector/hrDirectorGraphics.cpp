#include "stdafx.h"
#include "joystick.h"
#include "hrDirector.h"

HRESULT hrDirectorApp::CreateResources(ID2D1HwndRenderTarget* pRenderTarget)
{
	HRESULT hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pLightSlateGrayBrush
	);
	if (FAILED(hr))
		return hr;


	// Create a blue brush.
	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
		&m_pCornflowerBlueBrush
	);

	if (FAILED(hr))
		return hr;

	return hr;
}

void hrDirectorApp::DiscardResources()
{

	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

void hrDirectorApp::Update(DIJOYSTATE2 js)
{
	m_sizeA = 100 + (js.lX * 0.5f);
	m_sizeB = 100 + (js.lY * 0.5f);
}

void hrDirectorApp::Render(D2D1_SIZE_F rtSize, ID2D1HwndRenderTarget* pRenderTarget)
{

	RenderGrid(rtSize, pRenderTarget);

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


	pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

	// Draw the outline of a rectangle.
	pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);

}

void hrDirectorApp::RenderGrid(D2D1_SIZE_F rtSize, ID2D1HwndRenderTarget* pRenderTarget)
{

	/* Draw a grid background.*/
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	for (int x = 0; x < width; x += 10)
	{
		pRenderTarget->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
			m_pLightSlateGrayBrush,
			0.5f
		);
	}

	for (int y = 0; y < height; y += 10)
	{
		pRenderTarget->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
			m_pLightSlateGrayBrush,
			0.5f
		);
	}

}