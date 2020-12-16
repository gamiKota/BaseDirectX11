//**********************************************************************************
// �C���N���[�h��
//**********************************************************************************
#include "input.h"
#include "Frame.h"
#include "System.h"


//**********************************************************************************
// �}�N���E�萔��`
//**********************************************************************************
#define			WAIT_INPUT_REPEAT ((float)Frame::GetInstance().GetFrame() / 3)
static const int			KEYS_MASK = 0x8000;
#define INPUT_MAX 256


//**********************************************************************************
// �O���[�o���錾
//**********************************************************************************
static bool	g_keyInput[INPUT_MAX];			// ���͏��
static bool	g_keyOldInput[INPUT_MAX];		// 1�t���[���O�̓��͏��
static int	g_keyRepeatCount[INPUT_MAX];	// ���s�[�g�p�̃J�E���g
BYTE g_force[INPUT_MAX];

// �}�E�X
static HWND		g_mouseHWnd;
static HHOOK	g_mouseHook;
static int		g_wheelCount;
static int		g_wheelValue;
static bool		g_doubleClickMsg[3];
static bool		g_doubleClick[3];
static POINT	g_mousePos;


/**
 * @brief �}�E�X���b�Z�[�W�t�b�N
 */
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
	}

	MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	switch (pMsg->message)
	{
		// �z�C�[��
	case WM_MOUSEWHEEL:
		g_wheelCount += GET_WHEEL_DELTA_WPARAM(pMsg->wParam) / WHEEL_DELTA;
		break;
		// ���_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		g_doubleClickMsg[0] = true;
		break;
		// ���_�u���N���b�N
	case WM_MBUTTONDBLCLK:
		g_doubleClickMsg[1] = true;
		break;
		// �E�_�u���N���b�N
	case WM_RBUTTONDBLCLK:
		g_doubleClickMsg[2] = true;
		break;
	}
	return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}



/*
�ց@���F����������
�߂�l�FSuccess codes
*/
void Input::Init() {

	ZeroMemory(g_keyInput, sizeof(g_keyInput));
	ZeroMemory(g_keyOldInput, sizeof(g_keyOldInput));
	ZeroMemory(g_keyRepeatCount, sizeof(g_keyRepeatCount));
	memset(g_force, 255, INPUT_MAX);

	// �}�E�X
	g_mouseHWnd = GetActiveWindow();
	g_mouseHook = SetWindowsHookEx(WH_GETMESSAGE, MouseProc, 0, GetCurrentThreadId());
}


/*
�ց@���F�I������
*/
void Input::Uninit() {
	// ���ɏ����Ȃ�
	UnhookWindowsHookEx(g_mouseHook);
}


/*
�ց@���F�X�V����
*/
void Input::Update() {

	// �O�̃t���[���̓��͏��
	memcpy(g_keyOldInput, g_keyInput, sizeof(g_keyOldInput));

	//�L�[��������
	//������ԂȂ�true,�񉟉���ԂȂ�false
	for (int i = 0; i < INPUT_MAX; i++) {
		g_keyInput[i] = (GetAsyncKeyState(i) & KEYS_MASK) ? true : false;
	}

	// ���s�[�g�̃J�E���g
	for (int i = 0; i < INPUT_MAX; i++) {

		if (g_force[i] != 255)
		{
			g_keyInput[i] = g_force[i];
			g_force[i] = 255;
		}

		if (Input::isPress(i)) {
			if (g_keyRepeatCount[i] <= WAIT_INPUT_REPEAT) {
				g_keyRepeatCount[i]++;
			}
		}
		else {
			g_keyRepeatCount[i] = 0;
		}
	}

	//--- �}�E�X
	// �}�E�X���W
	GetCursorPos(&g_mousePos);
	ScreenToClient(g_mouseHWnd, &g_mousePos);
	// �}�E�X�z�C�[��
	g_wheelValue = g_wheelCount;
	g_wheelCount = 0;
	// �}�E�X�N���b�N
	for (int i = 0; i < 3; ++i)
	{
		g_doubleClick[i] = g_doubleClickMsg[i];
		g_doubleClickMsg[i] = false;
	}
}


/*
�ց@���F�L�[�v���X
���@���F���z�L�[�R�[�h
�߂�l�Ftrue  = �����̉��z�L�[�R�[�h��������Ă����
		false = �����̉��z�L�[�R�[�h��������Ă��Ȃ�
*/
bool Input::isPress(int nVertKey) {
	return g_keyInput[nVertKey];
}


/*
�ց@���F�L�[�g���K�[(�������Ƃ�
*/
bool Input::isTrigger(int nVertKey) {
	return (!g_keyOldInput[nVertKey] && g_keyInput[nVertKey]) ? true : false;
}


/*
�ց@���F�L�[�����[�X(�������Ƃ�
*/
bool Input::isRelease(int nVertKey) {
	return (g_keyOldInput[nVertKey] && !g_keyInput[nVertKey]) ? true : false;
}


/*
�ց@���F�L�[���s�[�g(�����������Ƃ�
*/
bool Input::isRepeat(int nVertKey) {
	if (g_keyRepeatCount[nVertKey] == 1) {						// �ŏ��̈���
		return true;
	}
	else if (g_keyRepeatCount[nVertKey] > WAIT_INPUT_REPEAT) {	// �����������Ƃ�
		return true;
	}
	return false;												// ������ĂȂ� or �܂����������Ă��
}


void Input::InstantForce(int nKey, bool bPress) {
	g_force[nKey] = bPress ? 0x80 : 0;
}

bool Input::IsMouseDoubleClick(int nKey) {
	switch (nKey)
	{
	default: return false;
	case VK_LBUTTON: return g_doubleClick[0];
	case VK_MBUTTON: return g_doubleClick[1];
	case VK_RBUTTON: return g_doubleClick[2];
	}
}


const POINT& Input::GetMousePos() {
	return g_mousePos;
}


int Input::GetMouseWheel() {
	return g_wheelValue;
}


// EOF