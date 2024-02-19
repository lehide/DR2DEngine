#include "DR2DEngine.h"
#include <stdio.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")	// writefactory


DR2DEngine::DR2DEngine()
	: m_hWnd(nullptr),
	m_pD2DFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr),
	m_pRenderTarget(nullptr),
	m_pWhiteBrush(nullptr), m_pYellowBrush(nullptr), m_pTempBrush(nullptr), m_pNowBrush(nullptr),
	m_IsDDResReady(S_FALSE), m_RenderTargetSize(D2D1_SIZE_F())
{

}

DR2DEngine::~DR2DEngine()
{
	// �� �ڿ����� ������� ������ Ÿ�̹��� Finalize()�� �Ű���.

	// ������� Finalize() ȣ�� ���Ŀ� ���� �Ҹ� �� �ʿ��� ó��
}

void DR2DEngine::Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight)
{
	/// Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
	HRESULT _hResult = CoInitialize(NULL);
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	m_hWnd = hWnd;

	HRESULT hr = S_OK;

	// 1. Direct2D Factory�� �����.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	static const WCHAR msc_fontName[] = L"����";
	static const FLOAT msc_fontSize = 12;

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_pTextFormat
		);
	}

	// �ؽ�Ʈ ���� ���
	if (SUCCEEDED(hr))
	{
		// Left-Top the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	}

}

void DR2DEngine::ChangeDisplay(int screenWidth, int screenHeight)
{
}

void DR2DEngine::RestoreDisplay()
{
}

void DR2DEngine::BeginRender()
{
	m_IsDDResReady = CreateDeviceResources();

	// ����̽� ������ �ڿ����� ���� ����, �������� �� �� �ִ� ��Ȳ�� ��
	if (SUCCEEDED(m_IsDDResReady) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		m_RenderTargetSize = m_pRenderTarget->GetSize();

		// �׸���� BegineDraw()�� EndDraw()���̿�
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
}

void DR2DEngine::EndRender()
{
	if (SUCCEEDED(m_IsDDResReady) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// �׸���� BegineDraw()�� EndDraw()���̿�
		HRESULT hr = m_pRenderTarget->EndDraw();

		// �ڿ��� �����ؾ� �� ���
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
}

HRESULT DR2DEngine::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	// 2. Direct2D ���� Ÿ���� �����Ѵ�.
	if (m_pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			// Create brushes.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 0.f), &m_pYellowBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f), &m_pNowBrush);
		}

	}

	return hr;
}

void DR2DEngine::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pYellowBrush);
}

void DR2DEngine::Finalize()
{
	SafeRelease(&m_pD2DFactory);	//m_pD2DFactory->Release();
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pYellowBrush);

	CoUninitialize();
}

void DR2DEngine::SetBrushColor(float r, float g, float b, float a)
{
	SafeRelease(&m_pNowBrush);

	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(r, g, b, a)), &m_pNowBrush);
}

void DR2DEngine::SetBrushColor(unsigned long dwRGBColor)
{
	SafeRelease(&m_pNowBrush);

	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(dwRGBColor)), &m_pNowBrush);
}

void DR2DEngine::DrawRectangle(float x, float y, float x1, float y1, COLORREF crBrush)
{
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(crBrush)), &m_pTempBrush);
	
	if (m_pTempBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = x;
		_rt.top = y;
		_rt.right = x1;
		_rt.bottom = y1;

		m_pRenderTarget->DrawRectangle(_rt, m_pTempBrush, 1.0f);
	}

	SafeRelease(&m_pTempBrush);
}

void DR2DEngine::DrawRectangle(float x, float y, float x1, float y1)
{
	D2D1_RECT_F _rt;
	_rt.left = x;
	_rt.top = y;
	_rt.right = x1;
	_rt.bottom = y1;

	m_pRenderTarget->DrawRectangle(_rt, m_pNowBrush, 2.0f);
}

void DR2DEngine::DrawLine(float x1, float y1, float x2, float y2)
{
	D2D1_POINT_2F _point;
	_point.x = x1;
	_point.y = y1;
	D2D1_POINT_2F _point2;
	_point2.x = x2;
	_point2.y = y2;

	m_pRenderTarget->DrawLine(_point, _point2, m_pNowBrush, 1.0f);
}

void DR2DEngine::DrawEllipse(int x, int y, int size)
{
	this->DrawEllipse(x, y, x + size, y + size);
}

void DR2DEngine::DrawEllipse(int left, int top, int right, int bottom)
{
	D2D1_ELLIPSE _region;
	_region.point.x = (float)left;
	_region.point.y = (float)top;
	_region.radiusX = (float)(right - left);
	_region.radiusY = (float)(bottom - top);

	m_pRenderTarget->DrawEllipse(_region, m_pNowBrush);
}

void DR2DEngine::DrawSelectRectangle(float x1, float y1, float x2, float y2)
{
	this->DrawRectangle(x1, y1, x2, y2, COLORREF(RGB(0, 255, 0)));
}

void DR2DEngine::DrawSelectedCircle(int x, int y, int size)
{
	this->DrawEllipse(x, y, x + size, y + size);
}

bool DR2DEngine::DrawText(int x, int y, const char* pch, ...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��
	char* buffer;
	int len;

	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// �翬�� ��Ƽ����Ʈ�� �����ڵ��� ���ڿ� ���� ���̰� �ִ�. 2����Ʈ ���� ��
	::MultiByteToWideChar(CP_ACP, 0, buffer, (int)strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// ��Ʈ�� �����ϴ� ���� ��� �ʿ�
		m_pRenderTarget->DrawText(unicodestr, (UINT32)wcslen(unicodestr), m_pTextFormat,
			//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
			D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
			m_pNowBrush);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// ���˰��� ���� ����
	delete[] buffer;

	return true;
}

void DR2DEngine::SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix)
{
	m_pRenderTarget->SetTransform(transformMatrix);

}
