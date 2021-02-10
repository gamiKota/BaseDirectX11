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
	PlayerState*		m_state;		//!< ��ԊǗ�
	PlayerOperation*	m_operate;		//!< ����Ǘ�
	PlayerBullet*		m_bullet;		//!< �e����
	PlayerHP*			m_hp;			//!< �̗͏���


public:
	void Start();
	void Update();
	void LastUpdate();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();
};


// EOF