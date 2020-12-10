/**
 * @file Laser.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Bullet.h"


/**
 * @class Bullet : inheritance Character
 */
class Laser : public Bullet {
public:
	void Start();
	void Update();

	void OnCollision(GameObject* obj);
};


// EOF