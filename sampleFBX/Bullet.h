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
#include "Component.h"


/**
 * @class Bullet : inheritance Component
 */
class Bullet : public Component
{
private:
	int			m_nStat;		// 0:未使用, 1:使用中, 2:爆発中
	int			m_nLife;		// 寿命

public:

	void Start();		// 初期化
	void Update();		// 更新

	void OnCollision(GameObject* obj);
	void Fire(Transform transform);	// 発射
};


// EOF