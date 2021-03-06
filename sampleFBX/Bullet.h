/**
 * @file Bullet.h
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
 * @class Bullet : inheritance Character
 */
class Bullet : public Character {
protected:
	float	m_nLife;	//!< 寿命

public:

	void Start();		// 初期化
	void Update();		// 更新

	void OnCollision(GameObject* obj);
};


// EOF