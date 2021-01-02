/**
 * @file Status.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


struct StatusVal {
	float data;	//!< �f�[�^
	float min;	//!< �f�[�^�������ŏ��l
	float max;	//!< �f�[�^�������ő�l

	StatusVal() {
		data = min = max = 0.f;
	}
	StatusVal(float _data, float _max, float _min = 0.f) {
		data = _data;
		min = _min;
		max = _max;
	}
	void InitData() {
		data = min;
	}
};


/**
 * @class Status : inheritance Component
 * @brief �e�I�u�W�F�N�g(�~�T�C���Ȃ�)�Ɏ�������
 */
class Status : public Component {
public:	// �X�e�[�^�X
	float	m_HP;			//!< �̗�
	float	m_AttakPower;	//!< �U����

public:
	float	m_targetDist;	//!< �^�[�Q�b�g�Ƃ̋���

	// �ő�l�܂ł̉��Z��Status�N���X�Ŏ��s
	// �l�������͊e�R���|�[�l���g�Ŏ��s
public:	// ����
	StatusVal	m_bulletTime;	//!< �e���ˊǗ�
	StatusVal	m_deleteTime;	//!< ���S�o�ߎ���
	StatusVal	m_hitStopTime;	//!< ��e����

public:	// �t���O
	bool	m_isDead;		//!< ���S�t���O


public:
	Status();
	void Start();
	void Update();
	void SetImGuiVal();
};


// EOF