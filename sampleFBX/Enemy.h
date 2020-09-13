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
#include "Component.h"


/**
 * @class Enemy : inheritance Component
 */
class Enemy : public Component {
public:
	void Start();
	void Update();
};


// EOF