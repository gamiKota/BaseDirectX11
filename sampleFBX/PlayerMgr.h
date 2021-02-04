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


class PlayerState;
class PlayerOperation;


/**
 * @class PlayerMgr : inheritance Character
 */
class PlayerMgr : public Character
{
public:
	PlayerState* m_state;		//!< ó‘ÔŠÇ—
	PlayerOperation* m_operate;	//!< ‘€ìŠÇ—


public:
	void Start();
	void Update();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();
};


// EOF