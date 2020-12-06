/**
 * @file Character.h
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
 * @forward declaration
 */
class Status;
class Collision;
class Rigidbody;


/**
 * @class Character : inheritance Component
 */
class Character : public Component {
public:
	void Init();

public:
	Status*		m_status;		//!< �X�e�[�^�X
	Collision*	m_collision;	//!< �����蔻��
	Rigidbody*	m_rigidbody;	//!< �����G���W���@�\
};


// EOF