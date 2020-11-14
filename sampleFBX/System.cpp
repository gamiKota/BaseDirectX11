//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "Frame.h"		// �t���[���Ǘ�
#include "Graphics.h"	// �`��
#include "System.h"		// �錾�N���X��


//********************************************************************************
// �}�N���E�萔��`
//********************************************************************************
#define CLASS_NAME		_T("AppClass")					// �E�C���h�E�̃N���X��
#define WINDOW_NAME		_T("�R���|�[�l���g�w��")		// �E�C���h�E�̃L���v�V������
static const Library LIBRARY_MODE = Library::DirectX;	// ���C�u�������[�h
static const BOOL CURSOR_MODE = TRUE;


//********************************************************************************
// �}�N���E�萔��`
//********************************************************************************
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int	OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);


bool System::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	// �E�B���h�E�̏�����
	this->InitializeWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// �t���[���̏�����
	Frame::GetInstance().Init();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	//if (Graphics::GetInstance()) {
	//	return false;
	//}
	// Initialize the graphics object.
	if (FAILED(Graphics::GetInstance().Init(Library::DirectX, 0, 0, m_hwnd))) {
		return false;
	}

	return true;
}


void System::Shutdown() {
	// �t���[���@�\�̏I������
	Frame::GetInstance().Uninit();

	// Release the graphics object.
	Graphics::GetInstance().Shutdown();

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, m_hinstance);

	// COM�I������
	CoUninitialize();

	return;
}


void System::Run() {
	MSG msg;

	// ���b�Z�[�W���[�v
	for (;;) {
		// �Q�[���I��
		if (0/* �����Q�[���̏I������ */) {
			break;
		}

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			Frame::GetInstance().Update();
			if (Frame::GetInstance().isUpdateGame()) {
				Graphics::GetInstance().GetInstance().Update();	// �X�V����
				Graphics::GetInstance().GetInstance().Draw();	// �`�揈��
			}
		}
	}
}


bool System::InitializeWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		nullptr,
		LoadCursor(nullptr, IDC_WAIT),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		nullptr
	};
	ShowCursor(CURSOR_MODE);

	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBoxW(NULL, L"COM�̏������Ɏ��s���܂����B", L"error", MB_OK);
		return false;
	}

	// �C���X�^���X �n���h���ۑ�
	m_hinstance = hInstance;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�T�C�Y����E�B���h�E �T�C�Y�Z�o
	DWORD dwStyle = 0;
	DWORD dwExStyle = 0;
	RECT  rc;
	dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	// �E�B���h�E�̍쐬
	m_hwnd = CreateWindowEx(dwExStyle,
		CLASS_NAME,
		WINDOW_NAME,
		dwStyle,
		1,		// �E�B���h�E�̍����W
		1,		// �E�B���h�E�̏���W
		rc.right - rc.left,	// �E�B���h�E����
		rc.bottom - rc.top,	// �E�B���h�E�c��
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	return true;
}


HINSTANCE System::GetWinInstance() {
	return m_hinstance;
}


HWND System::GetWnd() {
	return m_hwnd;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:										//----- �E�B���h�E���������ꂽ
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:									//----- �E�B���h�E�j���w��������
		PostQuitMessage(0);								// �V�X�e���ɃX���b�h�̏I����v��
		break;
	case WM_KEYDOWN:									//----- �L�[�{�[�h�������ꂽ
		switch (wParam) {
		case VK_ESCAPE:									// [ESC]�L�[�������ꂽ
			PostMessage(hWnd, WM_CLOSE, 0, 0);			// [x]�������ꂽ�悤�ɐU����
			return 0;
		}
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;							// [Alt]+[Enter]����BEEP��}�~
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//=============================================================================
// WM_CREATE���b�Z�[�W�n���h��
//=============================================================================
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// �N���C�A���g�̈�T�C�Y��SCREEN_WIDTH�~SCREEN_HEIGHT�ɍĐݒ�.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) {
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	// IME������
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1��Ԃ���CreateWindow[Ex]�����s����.
}


// EOF