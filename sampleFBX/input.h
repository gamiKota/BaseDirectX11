#ifndef ___HEADER_INPUT_H___
#define ___HEADER_INPUT_H___


//**********************************************************************************
// �C���N���[�h��
//**********************************************************************************
#include <Windows.h>


namespace Input
{
	void Init();
	void Uninit();
	void Update();
	/*
	void DrawInput();
	���͂ŕ`�悷����̂͂Ȃ�...
	���A�f�o�b�O�\����i�Q�[�̓��͊m�F�Ȃ�
	�Q�[���ɂ���ėp�r���ς��
	*/

	/*
	�ց@���F�L�[�v���X
	���@���F���z�L�[�R�[�h
	�߂�l�Ftrue  = �����̉��z�L�[�R�[�h��������Ă����
	false = �����̉��z�L�[�R�[�h��������Ă��Ȃ�
	*/
	bool isPress(int nVertKey);


	/*
	�ց@���F�L�[�g���K�[(�������Ƃ�
	*/
	bool isTrigger(int nVertKey);


	/*
	�ց@���F�L�[�����[�X(�������Ƃ�
	*/
	bool isRelease(int nVertKey);


	/*
	�ց@���F�L�[���s�[�g(�����������Ƃ�
	*/
	bool isRepeat(int nVertKey);


	void InstantForce(int nKey, bool bPress);

	bool IsMouseDoubleClick(int nKey);
	const POINT& GetMousePos();
	int GetMouseWheel();
};



#endif // !___HEADER_INPUT_H___
// EOF