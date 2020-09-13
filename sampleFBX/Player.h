/**
 * @file Player.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "Transform.h"


/**
 * @constant
 */
static const float MAX_MOVE_WIDTH = 1000.f;
static const float VAL_MOVE_PLAYER = 0.f;


/**
 * @class Player : inheritance Component
 */
class Player : public Component
{
public:
	float3			m_vMove;	//!< �ړ���
	float			m_roll;		//!< �X��(Z��)
	GameObject*		m_target;	//!< �^�[�Q�b�g

public:
	void Start();
	void Update();
};


// EOF