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


/**
 * @class Status : inheritance Component
 * @brief �e�I�u�W�F�N�g(�~�T�C���Ȃ�)�Ɏ�������
 */
class Status : public Component {
public:	// ���������X�e�[�^�X
	float	m_HP;			//!< �̗�
	float	m_AttakPower;	//!< �U����
	float	m_HitStop;		//!< ��e����
	bool	m_isDead;		//!< ���S�t���O

public:

public:
	Status();
	void Start();
	void Update();
	void SetImGuiVal();
};


// EOF