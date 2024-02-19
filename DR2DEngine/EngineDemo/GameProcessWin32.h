//
// 게임 진행
// 
// 최외곽의 가장 큰 클래스이다.
// 내부에 WinAPI32 모듈 (윈도 생성과 메시지 콜백 처리 등),
// 2D 그래픽스 엔진 등을 포함한다.
//
// [2/19/2024 LeHide]
// 

#pragma once

#include <windows.h>
#include <vector>

// 외부 종속성으로 동작한다.
#include "DR2DEngine.h"	// Direct2D를 이용한 그래픽스 엔진
#include "CGameObject.h"

using namespace std;

class GameProcessWin32
{
public:
	GameProcessWin32();
	~GameProcessWin32();

private:
	// 공통적인 설정

	// 화면의 크기
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;

	// 그래픽스 엔진
	IDREngine* m_pDREngine;

	// 충돌체크 테스트
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
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();

	// 게임을 정리한다.
	void Finalize();


	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// 게임 진행 클래스는 Win32와는 독립적이어야 한다.
// 하지만, Win32의 일부 설정을 wWinMain함수에서 받아와서 넣어 줘야 한다.
// 엔트리포인트에서부터 GameProcess의 종속성은 깨지게된다.
// 어차피 게임 진행 클래스를 재사용할것이라고는 생각치 않지만..
// 여튼, Win32부분을 어떻게든 모아보고 싶었다. 만약, 이것을 추상화한다면 UWP나 리눅스 코드로도 사용할 수 있어야 하는데,
// 그렇게 될 리가 없지. 그러므로 이 게임 진행 클래스는 Win32에 종속적인 것이 맞다.
// 그러므로 이름이 Win32가 관련되어야만다.