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
#include "Character.h"


/**
 * @constant
 */
static const float MAX_MOVE_WIDTH = 1000.f;
static const float VAL_MOVE_PLAYER = 0.f;


/**
 * @class PlayerMgr : inheritance Character
 */
class PlayerMgr : public Character
{
public:
	float3			m_vMove;	//!< ˆÚ“®—Ê
	float			m_roll;		//!< ŒX‚«(ZŽ²)
	GameObject*		m_target;	//!< ƒ^[ƒQƒbƒg

public:
	void Start();
	void Update();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();

private:
	void Operation();
};


// EOF