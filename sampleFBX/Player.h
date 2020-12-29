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
	float3			m_vMove;	//!< �ړ���
	float			m_roll;		//!< �X��(Z��)
	GameObject*		m_target;	//!< �^�[�Q�b�g

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