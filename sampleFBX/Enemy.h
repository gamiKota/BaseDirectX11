/**
 * @file Enemy.h
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
 * @class Enemy : inheritance Character
 */
class Enemy : public Character {
private:
	float m_deleteTime;		//!< 消滅時間

protected:
	GameObject* m_LockIcon;	//!< ロックオンマーカー

public:
	void Start();
	void Update();
	void Uninit();
};


// EOF