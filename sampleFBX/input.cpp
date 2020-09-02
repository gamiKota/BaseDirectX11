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
static const unsigned char INPUT_MAX = 255;


//**********************************************************************************
// �O���[�o���錾
//**********************************************************************************
static bool	g_keyInput[INPUT_MAX];			// ���͏��
static bool	g_keyOldInput[INPUT_MAX];		// 1�t���[���O�̓��͏��
static int	g_keyRepeatCount[INPUT_MAX];	// ���s�[�g�p�̃J�E���g


/*
�ց@���F����������
�߂�l�FSuccess codes
*/
void Input::Init() {

	ZeroMemory(g_keyInput, sizeof(g_keyInput));
	ZeroMemory(g_keyOldInput, sizeof(g_keyOldInput));
	ZeroMemory(g_keyRepeatCount, sizeof(g_keyRepeatCount));
}


/*
�ց@���F�I������
*/
void Input::Uninit() {
	// ���ɏ����Ȃ�
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
		if (Input::isPress(i)) {
			if (g_keyRepeatCount[i] <= WAIT_INPUT_REPEAT) {
				g_keyRepeatCount[i]++;
			}
		}
		else {
			g_keyRepeatCount[i] = 0;
		}
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


// EOF