//
// EngineDemo
//
// Darong Direct2D ������ ������ ����
//
// [2/19/2024 LeHideHome]
//

#include <windows.h>
#include "GameProcessWin32.h"

// ���α׷��� ��Ʈ�� ����Ʈ. �����ڵ�
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// ���� ���� Ŭ����
	// �ֿܰ��� ���� ū Ŭ�����̴�.
	// ���ο� Win32 ��� (���� ������ �޽��� �ݹ��� ó����),
	// 2D �׷��Ƚ� ���� ���� �����Ѵ�.
	GameProcessWin32* pGameProcess = new GameProcessWin32();

	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();
	
	delete pGameProcess;

	return 0; // (int)msg.wParam;
}
