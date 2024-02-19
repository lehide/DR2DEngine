//
// ���� ����
// 
// �ֿܰ��� ���� ū Ŭ�����̴�.
// ���ο� WinAPI32 ��� (���� ������ �޽��� �ݹ� ó�� ��),
// 2D �׷��Ƚ� ���� ���� �����Ѵ�.
//
// [2/19/2024 LeHide]
// 

#pragma once

#include <windows.h>
#include <vector>

// �ܺ� ���Ӽ����� �����Ѵ�.
#include "DR2DEngine.h"	// Direct2D�� �̿��� �׷��Ƚ� ����
#include "CGameObject.h"

using namespace std;

class GameProcessWin32
{
public:
	GameProcessWin32();
	~GameProcessWin32();

private:
	// �������� ����

	// ȭ���� ũ��
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;

	// �׷��Ƚ� ����
	IDREngine* m_pDREngine;

	// �浹üũ �׽�Ʈ
	CGameObject* m_pTargetObject;
	CGameObject* m_pMyObject;

private:
	// for Loop
	void UpdateAll();
	void RenderAll();

	bool CollisionCheck(CGameObject* pTarget, CGameObject* pMine);

	bool m_KeyUp;
	bool m_KeyDown;
	bool m_KeyLeft;
	bool m_KeyRight;

public:
	// ���ӿ� ���� �����ӿ��� �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);

	// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
	void Loop();

	// ������ �����Ѵ�.
	void Finalize();


	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// ���� ���� Ŭ������ Win32�ʹ� �������̾�� �Ѵ�.
// ������, Win32�� �Ϻ� ������ wWinMain�Լ����� �޾ƿͼ� �־� ��� �Ѵ�.
// ��Ʈ������Ʈ�������� GameProcess�� ���Ӽ��� �����Եȴ�.
// ������ ���� ���� Ŭ������ �����Ұ��̶��� ����ġ ������..
// ��ư, Win32�κ��� ��Ե� ��ƺ��� �;���. ����, �̰��� �߻�ȭ�Ѵٸ� UWP�� ������ �ڵ�ε� ����� �� �־�� �ϴµ�,
// �׷��� �� ���� ����. �׷��Ƿ� �� ���� ���� Ŭ������ Win32�� �������� ���� �´�.
// �׷��Ƿ� �̸��� Win32�� ���õǾ�߸���.