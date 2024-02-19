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
	/// Win32 관련
	// 윈도 클래스
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

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, m_ScreenWidth, m_ScreenHeight,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);


	/// 그래픽스 엔진 관련

	// 그래픽스 엔진 초기화
	m_pDREngine = new DR2DEngine();

	m_pDREngine->Initialize(hInstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);


	/// 오브젝트 초기화
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
	// 키 입력
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


	// 오브젝트 이동
	if (m_KeyUp == true) m_pMyObject->Accelerate(0, -0.3f);
	if (m_KeyDown == true) m_pMyObject->Accelerate(0, 0.3f);
	if (m_KeyRight == true) m_pMyObject->Accelerate(0.3f, 0);
	if (m_KeyLeft == true) m_pMyObject->Accelerate(-0.3f, 0);




	// 충돌감지, 반사벡터를 이용해서 속도를 재지정함. MK1~MK3정도를 만들자.
	CollisionCheck(m_pTargetObject, m_pMyObject);

	// 충돌 체크 이후
	// 현재 나아가야 할 수정된 속도 벡터로 이동 처리를 최종적으로 해 준다.
	m_pTargetObject->Update();
	m_pMyObject->Update();
}

void GameProcessWin32::RenderAll()
{
	/// 그리기를 준비한다.
	m_pDREngine->BeginRender();


	// 문자 출력 테스트
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

	// 오브젝트를 그린다.
	m_pTargetObject->Render(m_pDREngine);
	m_pTargetObject->DrawCollisionData(m_pDREngine);

	m_pMyObject->Render(m_pDREngine);
	m_pMyObject->DrawCollisionData(m_pDREngine);


	/// 그리기를 끝낸다.
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

// 윈도 메시지 핸들러인 콜백 함수
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
