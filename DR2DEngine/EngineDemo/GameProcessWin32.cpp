#include "GameProcessWin32.h"


GameProcessWin32::GameProcessWin32()
	: m_hWnd(NULL), m_pDREngine(NULL), m_pTargetObject(NULL), m_pMyObject(NULL)
{

}

GameProcessWin32::~GameProcessWin32()
{

}

HRESULT GameProcessWin32::Initialize(HINSTANCE hInstance)
{
	/// Win32 ����
	// ���� Ŭ����
	wchar_t szAppName[] = L"LeHide 2024 Direct2D Demo";

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = GameProcessWin32::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ���� ����
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, m_ScreenWidth, m_ScreenHeight,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);


	/// �׷��Ƚ� ���� ����

	// �׷��Ƚ� ���� �ʱ�ȭ
	m_pDREngine = new DR2DEngine();

	m_pDREngine->Initialize(hInstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);


	/// ������Ʈ �ʱ�ȭ
	m_pTargetObject = new CGameObject();
	m_pMyObject = new CGameObject();

	m_pTargetObject->SetPosition(300, 300);
	m_pMyObject->SetPosition(400, 400);


	// Circle Collision (or Bounding Sphere)
	m_pTargetObject->SetCC(50.f);
	m_pMyObject->SetCC(70.f);

	// Axis-Aligned Bounding Box
	//m_pTargetObject->SetAABB(50.f, 50.f);
	//m_pMyObject->SetAABB(70.f, 70.f);


	// Oriented Bounding Box
	//m_pTargetObject->SetOBB(50.f, 80.f, 10.f);
	//m_pMyObject->SetOBB(100.f, 70.f, 5.f);

	return S_OK;
}

void GameProcessWin32::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;

			DispatchMessage(&m_Msg);
		}
		else
		{
			UpdateAll();
			RenderAll();
		}
	}
}

void GameProcessWin32::UpdateAll()
{
	// Ű �Է�
	if (GetAsyncKeyState(VK_UP))		m_KeyUp = true;		else m_KeyUp = false;
	if (GetAsyncKeyState(VK_DOWN))		m_KeyDown = true;	else m_KeyDown = false;
	if (GetAsyncKeyState(VK_RIGHT))		m_KeyRight = true;	else m_KeyRight = false;
	if (GetAsyncKeyState(VK_LEFT))		m_KeyLeft = true;	else m_KeyLeft = false;

	if (GetAsyncKeyState(VK_SPACE))
	{
		m_pMyObject->Rotate(1.0f);
		m_pTargetObject->Rotate(3.0f);
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
		m_pMyObject->Move_Stop();
		m_pTargetObject->Move_Stop();
	}


	// ������Ʈ �̵�
	if (m_KeyUp == true) m_pMyObject->Accelerate(0, -0.3f);
	if (m_KeyDown == true) m_pMyObject->Accelerate(0, 0.3f);
	if (m_KeyRight == true) m_pMyObject->Accelerate(0.3f, 0);
	if (m_KeyLeft == true) m_pMyObject->Accelerate(-0.3f, 0);




	// �浹����, �ݻ纤�͸� �̿��ؼ� �ӵ��� ��������. MK1~MK3������ ������.
	CollisionCheck(m_pTargetObject, m_pMyObject);

	// �浹 üũ ����
	// ���� ���ư��� �� ������ �ӵ� ���ͷ� �̵� ó���� ���������� �� �ش�.
	m_pTargetObject->Update();
	m_pMyObject->Update();
}

void GameProcessWin32::RenderAll()
{
	/// �׸��⸦ �غ��Ѵ�.
	m_pDREngine->BeginRender();


	// ���� ��� �׽�Ʈ
	m_pDREngine->SetBrushColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_pDREngine->DrawText(10, 10, "Direct2D Game Programming - 05 Physics");
	m_pDREngine->DrawText(630, 100, "Status");

	m_pDREngine->DrawText(630, 150, "ElapsedTime : %d", 16);
	m_pDREngine->DrawText(630, 170, "----------------------------");
	m_pDREngine->DrawText(630, 190, "targetObject Pos : %d / %d", m_pTargetObject->GetPosX(), m_pTargetObject->GetPosY());
	m_pDREngine->DrawText(630, 210, "myObject Pos : %d / %d", m_pMyObject->GetPosX(), m_pMyObject->GetPosY());
	m_pDREngine->DrawText(630, 230, "Magniture : %f", m_pMyObject->GetMagnitude());
	m_pDREngine->DrawText(630, 250, "%f/%f   %f/%f", m_pMyObject->m_OBB.edge1.x, m_pMyObject->m_OBB.edge1.y, m_pMyObject->m_OBB.edge2.x, m_pMyObject->m_OBB.edge2.y);
	//m_pDREngine->DrawText(630, 270, "%f/%f  %f/%f %f/%f %f/%f", );
	m_pDREngine->DrawText(630, 290, "----------------------------");

	// ������Ʈ�� �׸���.
	m_pTargetObject->Render(m_pDREngine);
	m_pTargetObject->DrawCollisionData(m_pDREngine);

	m_pMyObject->Render(m_pDREngine);
	m_pMyObject->DrawCollisionData(m_pDREngine);


	/// �׸��⸦ ������.
	m_pDREngine->EndRender();
}

bool GameProcessWin32::CollisionCheck(CGameObject* pTarget, CGameObject* pMine)
{
	if (pTarget->GetCollisionType() == CGameObject::eCollisionType::eCC &&
		pMine->GetCollisionType() == CGameObject::eCollisionType::eCC)
	{
		return pMine->CollisionCheck_CC2(pTarget);
	}
	else if (pTarget->GetCollisionType() == CGameObject::eCollisionType::eAABB &&
		pMine->GetCollisionType() == CGameObject::eCollisionType::eAABB)
	{
		return pMine->CollisionCheck_AABB(pTarget);
	}
	else if (pTarget->GetCollisionType() == CGameObject::eCollisionType::eOBB &&
		pMine->GetCollisionType() == CGameObject::eCollisionType::eOBB)
	{
		return pMine->CollisionCheck_OBB(pTarget);
	}

	return false;
}

void GameProcessWin32::Finalize()
{
	m_pDREngine->Finalize();

	delete m_pDREngine;
}

// ���� �޽��� �ڵ鷯�� �ݹ� �Լ�
LRESULT CALLBACK GameProcessWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
