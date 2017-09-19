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
		D2D1::ColorF(D2D1::ColorF::Magenta),
		&m_pGreenBrush
	);

	if (FAILED(hr))
		return hr;
	

	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Orange),
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

	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::OrangeRed),
		&m_pRedBrush
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
	SafeRelease(&m_pRedBrush);
}

void hrDirectorApp::Update(DIJOYSTATE2 js)
{

	long double x = sin(radian) + cos(halfradian);
	long double y = cos(radian) + sin(halfradian);
	radian += 0.0174532925 * 0.1;
	halfradian += (0.0174532925 * 0.06 );

	m_x = (float)(x*(GraticuleRadius / 2.0));
	m_y = (float)(y*(GraticuleRadius / 2.0));

	POINTF ps;

	SmoothJoystick(js, ps);
	m_x += (ps.x);
	m_y += -(ps.y);

	#ifdef _DEBUG 
	wchar_t buf[_MAX_PATH];
	wsprintfW(buf, L"%i - %i\n", (int)m_x, (int)m_y);
	OutputDebugString(buf);
#endif 
	float nSize = 30.0f;
	m_bFlag = (int)( ( m_x < -nSize) || (m_x > nSize) || (m_y < -nSize) || (m_y > nSize))?1:0;
	if (m_bFlag) {
		nSize += 30.0f;
		m_bFlag = (int)((m_x < -nSize) || (m_x > nSize) || (m_y < -nSize) || (m_y > nSize)) ?2 : 1;
	}

}


void hrDirectorApp::SmoothJoystick(DIJOYSTATE2 js, POINTF& ps)
{
	POINT accs = { 0 };

	for (int i = 0; i < JOYSTICK_SMOOTH_SIZE; i++)
	{
		accs.x += m_joystickpt[i].x - (LONG)GraticuleRadius;
		accs.y += m_joystickpt[i].y - (LONG)GraticuleRadius;
	}
	accs.x += js.lX;
	accs.y += js.lY;

	ps.x = (float)(accs.x / (JOYSTICK_SMOOTH_SIZE + 1));
	ps.y = (float)(accs.y / (JOYSTICK_SMOOTH_SIZE + 1));

	
	//POINT m_joystickpt[[JOYSTICK_SMOOTH_SIZE];

	for (int i = 1; i <= JOYSTICK_SMOOTH_SIZE; i++)
	{
		m_joystickpt[i-1].x = m_joystickpt[i].x;
		m_joystickpt[i-1].y = m_joystickpt[i].y;

	}
	m_joystickpt[JOYSTICK_SMOOTH_SIZE-1].x = js.lX + GraticuleRadius;
	m_joystickpt[JOYSTICK_SMOOTH_SIZE - 1].y = js.lY + GraticuleRadius;
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

	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0, 0), GraticuleRadius, GraticuleRadius), m_pBlackBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
//	pRenderTarget->DrawGeometry(m_pGraticule, m_pCornflowerBlueBrush,0.4f);

	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(0, 0), GraticuleRadius, GraticuleRadius), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);

	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(-10, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(-20, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(-30, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);

	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(10, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(20, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(30, 0), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);

	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,-10), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,-20), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,-30 ), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);

	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,10), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,20), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);
	pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(0,30), 2.0f, 2.0f), m_pWhiteBrush); // maxlen / 2.0f, maxlen / 2.0f), m_pCornflowerBlueBrush);


	CreateBars(m_pDirect2dFactory, m_x, m_y);
	ID2D1SolidColorBrush** barColor = &m_pGreenBrush;
	if (m_bFlag == 1)
		barColor = &m_pYellowBrush;

	if (m_bFlag == 2)
		barColor = &m_pRedBrush;
	pRenderTarget->DrawGeometry(m_pBars, *barColor, 2.0F);
	SafeRelease(&m_pBars);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

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


HRESULT hrDirectorApp::CreateBars(ID2D1Factory* pDirect2dFactory, float x, float y)
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
			pSink->BeginFigure(D2D1::Point2F(-GraticuleCase,y), D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLine(D2D1::Point2F(GraticuleCase, y));
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);

			pSink->BeginFigure(D2D1::Point2F(x,-GraticuleCase), D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLine(D2D1::Point2F(x,GraticuleCase));
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