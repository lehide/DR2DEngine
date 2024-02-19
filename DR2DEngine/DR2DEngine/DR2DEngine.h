#pragma once

/// Direct2d �׷��Ƚ� ����
///
/// �������̽��� ���ؼ�, WinAPI �׷��Ƚ� ������ ��ü �����ϴ�.
///
/// [11/16/2019 LeHideHome]

#include "IDREngine.h"
#include "DRMacro.h"
#include <d2d1.h>
#include <d2d1_1.h>
#include "d2d1helper.h"		// ColorF
#include <dwrite.h>			// writefactory


using namespace D2D1;

class DR2DEngine : public IDREngine
{
public:
	DR2DEngine();
	virtual ~DR2DEngine();

	/// IDREngine override
public:
	///--------------------------------------------------------------------------------
	/// �ʱ�ȭ ����

	// �ʱ�ȭ ����
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) override;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) override;
	virtual void RestoreDisplay() override;

	// Render ����
	virtual void BeginRender() override;

	// Render ��
	virtual void EndRender() override;

	// ������ �����Ѵ�.
	virtual void Finalize() override;

private:
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();

	///--------------------------------------------------------------------------------
	/// �׸��� ����

	// ������ �׸��� �귯�� ����
	virtual void SetBrushColor(float r, float g, float b, float a) override;
	virtual void SetBrushColor(unsigned long dwRGBColor) override;

	// ������ ��ǥ�� �簢���� ����Ѵ�. �̷������� ��ġ ���������� ���� �Ѵ�.
	virtual void DrawRectangle(float x, float y, float x1, float y1, COLORREF crBrush) override;
	virtual void DrawRectangle(float x, float y, float x1, float y1) override;
	virtual void DrawLine(float x1, float y1, float x2, float y2) override;
	virtual void DrawEllipse(int x, int y, int size) override;
	virtual void DrawEllipse(int left, int top, int right, int bottom) override;
	virtual void DrawSelectRectangle(float x1, float y1, float x2, float y2) override;
	virtual void DrawSelectedCircle(int x, int y, int size) override;

	// ȭ�鿡 ���ڸ� ���
	virtual bool DrawText(int x, int y, const char* pch, ...) override;

	/// TRS ����
	virtual void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix) override;

private:
	HWND m_hWnd;								// �׸��⸦ �� ���� (1����� ����)

	// Device Independant Resources
	ID2D1Factory* m_pD2DFactory;				// D2D�� Factory�� �����ϸ鼭 �����Ѵ�.

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;


	// Device Dependant Resources
	ID2D1HwndRenderTarget* m_pRenderTarget;		// �׸��⸦ �� ����� ���� Ÿ��


	// �귯�õ�
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pYellowBrush;
	ID2D1SolidColorBrush* m_pTempBrush;			// �׸��� �Լ� �������� ����� �뵵
	ID2D1SolidColorBrush* m_pNowBrush;			// �ٲ㰡�鼭 ���� �� ������ �뵵



private:
	// For render
	HRESULT m_IsDDResReady;				// ����̽� �������� �ڿ����� �غ�Ǿ��°�?
	D2D1_SIZE_F m_RenderTargetSize;

};

