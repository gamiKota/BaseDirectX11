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
class PlayerBullet;
class PlayerHP;


/**
 * @class PlayerMgr : inheritance Character
 */
class PlayerMgr : public Character
{
public:
	PlayerState*		m_state;		//!< ó‘ÔŠÇ—
	PlayerOperation*	m_operate;		//!< ‘€ìŠÇ—
	PlayerBullet*		m_bullet;		//!< ’eˆ—
	PlayerHP*			m_hp;			//!< ‘Ì—Íˆ—


public:
	void Start();
	void Update();
	void LastUpdate();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();
};


// EOF