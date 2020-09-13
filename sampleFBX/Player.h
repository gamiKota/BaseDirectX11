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
	float3			m_vMove;	//!< ˆÚ“®—Ê
	float			m_roll;		//!< ŒX‚«(ZŽ²)
	GameObject*		m_target;	//!< ƒ^[ƒQƒbƒg

public:
	void Start();
	void Update();
};


// EOF