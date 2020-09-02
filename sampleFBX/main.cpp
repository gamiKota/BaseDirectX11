/**
 * @file	main.cpp
 * @brief	�e�@�\�̃C���N���[�h
 *
 * @author	Kota Nakagami
 *
 * @version	1.00
 * @version	1.01	data1
 */


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "System.h"		// ���C���V�X�e��


//********************************************************************************
// ���C�u�����̃����N
//********************************************************************************
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")


//********************************************************************************
// �G���g���[�|�C���g
//********************************************************************************
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//! Variable declaration.
	bool result;

	// Initialize and run the system object.
	result = System::GetInstance().Initialize(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (result) {
		System::GetInstance().Run();	// Game loop.
	}
	else {
		MessageBoxA(NULL, "error", "error", MB_OK);
	}

	// Shutdown and release the system object.
	System::GetInstance().Shutdown();

	return 0;
}


// EOF