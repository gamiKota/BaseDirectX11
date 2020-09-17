/**
 * @file FixedEnemy.h
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
 * @class FixedEnemy : inheritance Enemy
 */
class FixedEnemy : public Enemy {
public:
	void Start();
	void Update();
};


// EOF