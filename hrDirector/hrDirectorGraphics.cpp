#include "stdafx.h"
#include "joystick.h"
#include "hrDirector.h"

HRESULT hrDirectorApp::CreateResources(ID2D1Factory* pDirect2dFactory, ID2D1HwndRenderTarget* pRenderTarget)
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

	// Create a blue brush.
	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DimGray),
		&m_pDimGrayBrush
	);

	if (FAILED(hr))
		return hr;

	// Create a blue brush.
	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&m_pBlackBrush
	);

	if (FAILED(hr))
		return hr;

	
	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&m_pGreenBrush
	);

	if (FAILED(hr))
		return hr;
	

	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pYellowBrush
	);

	if (FAILED(hr))
		return hr;

	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	);

	if (FAILED(hr))
		return hr;

	hr = CreateGraticule(pDirect2dFactory, GraticuleRadius);

	return hr;
}

void hrDirectorApp::DiscardResources()
{
	SafeRelease(&m_pBars);
	SafeRelease(&m_pGraticule);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&m_pDimGrayBrush);
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pYellowBrush);
	SafeRelease(&m_pWhiteBrush);
}

void hrDirectorApp::Update(DIJOYSTATE2 js)
{
	m_sizeA = 100 + (js.lX * 0.5f);
	m_sizeB = 100 + (js.lY * 0.5f);
}

void hrDirectorApp::Render(D2D1_SIZE_F rtSize, ID2D1HwndRenderTarget* pRenderTarget)
{

	//RenderGrid(rtSize, pRenderTarget);

	float maxlen = (rtSize.width < rtSize.height) ? rtSize.width : rtSize.height;

	D2D1::Matrix3x2F translationMatrixX = D2D1::Matrix3x2F::Translation(rtSize.width / 2, rtSize.height / 2);
	D2D1::Matrix3x2F scaleMatrixX = D2D1::Matrix3x2F::Scale(D2D1::SizeF((maxlen / 4) / 100.0f, (maxlen / 4) / 100.0f), D2D1::Point2F(0, 0));
	pRenderTarget->SetTransform(scaleMatrixX * translationMatrixX);

	D2D1_ROUNDED_RECT rectF = D2D1::RoundedRect(D2D1::RectF(-GraticuleCase, GraticuleCase, GraticuleCase, -GraticuleCase), 8.0f, 8.0f);
	pRenderTarget->FillRoundedRectangle(rectF, m_pDimGrayBrush);





	//pRenderTarget->SetTransform(translationMatrixX);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0, 0), GraticuleRadius, GraticuleRadius), m_pBlackBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
//	pRenderTarget->DrawGeometry(m_pGraticule, m_pCornflowerBlueBrush,0.4f);

	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(0, 0), GraticuleRadius, GraticuleRadius), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);

	//pRenderTarget->DrawLine(D2D1::Point2F(-GraticuleCase, 0), D2D1::Point2F(GraticuleCase, 0), m_pCornflowerBlueBrush, 1.0F);
	//pRenderTarget->DrawLine(D2D1::Point2F(0, -GraticuleCase), D2D1::Point2F(0, GraticuleCase), m_pCornflowerBlueBrush, 1.0F);
	CreateBars(m_pDirect2dFactory);
	pRenderTarget->DrawGeometry(m_pBars, m_pGreenBrush, 2.0F);
	SafeRelease(&m_pBars);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());




	/* Draw two rectangles.
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		maxlen / 2 - m_sizeA,
		maxlen / 2 - m_sizeA,
		maxlen / 2 + m_sizeA,
		maxlen / 2 + m_sizeA
	);

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		maxlen / 2 - m_sizeB,
		maxlen / 2 - m_sizeB,
		maxlen / 2 + m_sizeB,
		maxlen / 2 + m_sizeB
	);


	pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

	// Draw the outline of a rectangle.
	pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);
	*/

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


HRESULT hrDirectorApp::CreateBars(ID2D1Factory* pDirect2dFactory)
{
	ID2D1GeometrySink *pSink = NULL;
	ID2D1PathGeometry* tempPath;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&m_pBars);
	if (SUCCEEDED(hr))
	{
		hr = pDirect2dFactory->CreatePathGeometry(&tempPath);
	}
	if (SUCCEEDED(hr))
	{
		hr = tempPath->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{
			pSink->BeginFigure(D2D1::Point2F(-GraticuleCase,100), D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLine(D2D1::Point2F(GraticuleCase, 100));
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);

			pSink->BeginFigure(D2D1::Point2F(0,-GraticuleCase), D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLine(D2D1::Point2F(0,GraticuleCase));
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);

		hr = pSink->Close();
		SafeRelease(&pSink);
	}
	ID2D1GeometrySink* pSinkClip;
	if (SUCCEEDED(hr))
	{
		hr = m_pBars->Open(&pSinkClip);
	}

	D2D1_ELLIPSE clip;
	ID2D1EllipseGeometry* clipEllipse;
	if (SUCCEEDED(hr))
	{
		//Create a clip ellipse.
		clip = D2D1::Ellipse(
			D2D1::Point2F(0.0f, 0.0f),
			GraticuleRadius-1.0f, GraticuleRadius-1.0f
			
		);
		hr = pDirect2dFactory->CreateEllipseGeometry(&clip, &clipEllipse);
	}
	if (SUCCEEDED(hr))
	{
		//There's no direct support for clipping path in Direct2D. So we can intersect a path with its clip instead.
		hr = tempPath->CombineWithGeometry(clipEllipse, /*D2D1_COMBINE_MODE_EXCLUDE*/D2D1_COMBINE_MODE_INTERSECT, NULL, 0, pSinkClip);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSinkClip->Close();
		SafeRelease(&pSinkClip);
		SafeRelease(&tempPath);
		SafeRelease(&clipEllipse);
	}
	return hr;

}


HRESULT hrDirectorApp::CreateGraticule(ID2D1Factory* pDirect2dFactory, float length)
{


	ID2D1GeometrySink *pSink = NULL;
	ID2D1PathGeometry* tempPath;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&m_pGraticule);
	if (SUCCEEDED(hr))
	{
		hr = pDirect2dFactory->CreatePathGeometry(&tempPath);
	}
	if (SUCCEEDED(hr))
	{
		hr = tempPath->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{
		/* 2 degree divisions
		for (double angle = 2.0; angle <= 360.0; angle += 2.0)
		{
		if (angle < 120.f || angle > 240.f)
		{
		if (fmod(angle, 10.0) != 0.0)
		{

		float x1 = 0, x2 = 0, y1 = 0, y2 = -150; //0 degrees at top
		RotateLine(x1, y1, x2, y2, angle);
		pSink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_HOLLOW);
		pSink->AddLine(D2D1::Point2F(x2, y2));
		pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		}
		}
		}

		for (double angle = 4.0; angle <= 360.0; angle += 4.0)
		{
		//if (angle < 120.f || angle > 240.f)
		//{
		if (fmod(angle, 40.0) != 0.0)
		{

		float x1 = 0, x2 = 0, y1 = 0, y2 = -150; //0 degrees at top
		RotateLine(x1, y1, x2, y2, angle);
		pSink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_HOLLOW);
		pSink->AddLine(D2D1::Point2F(x2, y2));
		pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		}
		//}
		}*/

		/* 3.6 degree */
		for (double angle = 0; angle <= 360.0; angle += 3.6)
		{
			float x1 = 0, y1 = 0, x2 = 0, y2 = length;
			RotateLine(x1, y1, x2, y2, angle);
			pSink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLine(D2D1::Point2F(x2, y2));
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		}

		hr = pSink->Close();
		SafeRelease(&pSink);
	}
	ID2D1GeometrySink* pSinkClip;
	if (SUCCEEDED(hr))
	{
		hr = m_pGraticule->Open(&pSinkClip);
	}

	D2D1_ELLIPSE clip;
	ID2D1EllipseGeometry* clipEllipse;
	if (SUCCEEDED(hr))
	{
		//Create a clip ellipse.
		clip = D2D1::Ellipse(
			D2D1::Point2F(0, 0),
			length - (length / 8),
			length - (length / 8)
		);
		hr = pDirect2dFactory->CreateEllipseGeometry(&clip, &clipEllipse);
	}
	if (SUCCEEDED(hr))
	{
		//There's no direct support for clipping path in Direct2D. So we can intersect a path with its clip instead.
		hr = tempPath->CombineWithGeometry(clipEllipse, D2D1_COMBINE_MODE_EXCLUDE /*D2D1_COMBINE_MODE_INTERSECT*/, NULL, 0, pSinkClip);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSinkClip->Close();
		SafeRelease(&pSinkClip);
		SafeRelease(&tempPath);
		SafeRelease(&clipEllipse);
	}
	return hr;
}

void hrDirectorApp::RotateLine(float& x1, float& y1, float& x2, float& y2, double deg)
{
	int r, c, k;
	double rad, a[2][2], m[2][2], rot[2][2];
	a[0][0] = x1;
	a[0][1] = y1;
	a[1][0] = x2;
	a[1][1] = y2;
	rad = (deg*(3.14159265358979323846)) / 180;
	rot[0][0] = cos(rad);
	rot[0][1] = sin(rad);
	rot[1][0] = -sin(rad);
	rot[1][1] = rot[0][0];  //cos(rad)
	for (r = 0; r < 2; r++)
		for (c = 0; c < 2; c++)
		{
			m[r][c] = 0;
			for (k = 0; k < 2; k++)
				m[r][c] = m[r][c] + a[r][k] * rot[k][c];
		}
	x1 = (float)m[0][0], y1 = (float)m[0][1], x2 = (float)m[1][0], y2 = (float)m[1][1];

}