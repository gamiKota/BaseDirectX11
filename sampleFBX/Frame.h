#pragma once


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include <windows.h>
#include <tchar.h>
#include "Singleton.h"


/**
 * @class	Frame
 * @brief	�t���[���Ǘ�
 */
class Frame : public Singleton<Frame>
{
public:

	/**
	 * @brief	������
	 */
	void Init();

	/**
	 * @brief	�I������
	 */
	void Uninit();

	/**
	 * @brief	�X�V����
	 */
	void Update();

	/**
	 * @brief	�Q�[���X�V����
	 * @return	true = �Q�[���X�V
	 */
	bool isUpdateGame();

	/**
	 * @brief	�t���[���̎擾
	 * @return	DWORD FPS�J�E���g
	 */
	DWORD GetFrame();

	/**
	 * @brief	�Q�[���J�n����̌o�ߎ��Ԃ̎擾
	 * @return	DWORD �o�ߎ���
	 */
	DWORD GetPassageTime();

	/**
	 * @brief	FPS�J�E���g�̕`��
	 */
	void DrawFPS();

private:
	DWORD	m_startTime;	//!< �A�v���P�[�V�����N�����̎���
	DWORD	m_oldTime;		//!< �O�̎���
	DWORD	m_nowTime;		//!< ���̎���
	DWORD	m_Frame;		//!< �t���[����
};


// EOF