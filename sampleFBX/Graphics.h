/**
 * @file	Graphics.h
 * @brief	3D�O���t�B�b�N�X�̕`��ꊇ�Ǘ�
 *
 * @author	Kota Nakagami
 * @data	2019/12/06(��)
 *
 * @version	1.00
 */
#pragma once

 //********************************************************************************
 // �C���N���[�h��
 //********************************************************************************
#include <Windows.h>
#include "Singleton.h"


//********************************************************************************
// �}�N���E�萔��`
//********************************************************************************
#define		SCREEN_WIDTH	(1280)							// �E�C���h�E�̕�
#define		SCREEN_HEIGHT	(720)							// �E�C���h�E�̍���
#define		SCREEN_CENTER_X	(SCREEN_WIDTH/2)				// �E�C���h�E�̒��S�w���W
#define		SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)				// �E�C���h�E�̒��S�x���W
#define		SCREEN_RATIO	(SCREEN_WIDTH / SCREEN_HEIGHT)	// ��ʔ�


/**
 * @enum	class Library
 * @brief	3D�O���t�B�b�N�X�`��@�\�̎��
 *			�}�N���ŋ@�\�𕪂��������ǂ���
 */
enum class Library
{
	DirectX,	// DirectX11
	OpenGL,		// OpenGL
};


/**
 * @class	Graphics
 * @brief	�`��Ǘ�
 */
class Graphics : public Singleton<Graphics> {
public:
	/**
	 * @brief ���C�u�����ƃQ�[���̏�����
	 */
	bool Init(Library, int, int, HWND);

	/**
	 * @brief �Q�[���̍X�V
	 */
	void Update();

	/**
	 * @brief �Q�[���̕`��
	 */
	void Draw();

	/**
	 * @brief ���C�u�����ƃQ�[���̏I������
	 */
	void Shutdown();

	/**
	 * @brief	FPS�J�E���g�ݒ�
	 * @param	FPSCount
	 */
	void SetFPSCount(int FPSCount);

private:
	Library m_library;		//!< ���C�u����
	int		m_nCountFPS;	//!< FPS�J�E���^
};


// EOF