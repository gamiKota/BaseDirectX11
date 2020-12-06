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
public:
	void Start();
	void Update();
};


// EOF