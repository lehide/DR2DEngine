//
// EngineDemo
//
// Darong Direct2D 엔진의 간단한 데모
//
// [2/19/2024 LeHideHome]
//

#include <windows.h>
#include "GameProcessWin32.h"

// 프로그램의 엔트리 포인트. 유니코드
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// 게임 진행 클래스
	// 최외곽의 가장 큰 클래스이다.
	// 내부에 Win32 모듈 (윈도 생성과 메시지 콜백을 처리등),
	// 2D 그래픽스 엔진 등을 포함한다.
	GameProcessWin32* pGameProcess = new GameProcessWin32();

	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();
	
	delete pGameProcess;

	return 0; // (int)msg.wParam;
}
