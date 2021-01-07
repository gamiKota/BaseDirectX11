/**
 * @file EnemyFixed.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Enemy.h"


/**
 * @class EnemyFixed : inheritance Enemy
 */
class EnemyFixed : public Enemy {
public:
	void Start();
	void Update();

	void OnCollision(GameObject* obj);
};


// EOF