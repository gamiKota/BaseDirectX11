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
	PlayerState*		m_state;		//!< 状態管理
	PlayerOperation*	m_operate;		//!< 操作管理
	PlayerBullet*		m_bullet;		//!< 弾処理
	PlayerHP*			m_hp;			//!< 体力処理


public:
	void Start();
	void Update();
	void LastUpdate();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();
};


// EOF