#pragma once
#include <windows.h>
#include <d2d1.h>

/// Darong 2D Engine Interface
///
/// ����ؼ� WinAPI, Direct2D���� ��� �����ϴ�.
/// ��� �߻�ȭ�� �Ǿ� ������, windows.h������ ����Ѵ�.
///
/// [12/27/2019 LeHide]
///

class IDREngine abstract
{
protected:
	static IDREngine* m_Instance;

public:
	//IDREngine();
	//~IDREngine();

	static IDREngine* GetInstance() { return m_Instance; }

	///--------------------------------------------------------------------------------
	/// �ʱ�ȭ, �׸���, ������

	// �ʱ�ȭ ����
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) abstract;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) abstract;
	virtual void RestoreDisplay() abstract;

	// Render ����
	virtual void BeginRender() abstract;

	// Render ��
	virtual void EndRender() abstract;

	// ������ �����Ѵ�.
	virtual void Finalize() abstract;


	///--------------------------------------------------------------------------------
	/// �׸��� ���� Primitive

	// ������ �׸��� �귯�� ����
	virtual void SetBrushColor(float r, float g, float b, float a) abstract;
	virtual void SetBrushColor(unsigned long dwRGBColor) abstract;

	// ������ ��ǥ�� �簢���� ����Ѵ�.
	virtual void DrawRectangle(float x, float y, float x1, float y1, COLORREF crBrush) abstract;
	virtual void DrawRectangle(float x, float y, float x1, float y1) abstract;
	virtual void DrawLine(float x1, float y1, float x2, float y2) abstract;
	virtual void DrawEllipse(int x, int y, int size) abstract;
	virtual void DrawEllipse(int left, int top, int right, int bottom) abstract;
	virtual void DrawSelectRectangle(float x1, float y1, float x2, float y2) abstract;
	virtual void DrawSelectedCircle(int x, int y, int size) abstract;

	// ȭ�鿡 ���ڸ� ���
	virtual bool DrawText(int x, int y, const char* pch, ...) abstract;

	/// TRS ����
	virtual void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix) abstract;
};

