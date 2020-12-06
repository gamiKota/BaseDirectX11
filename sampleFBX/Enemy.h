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
	float m_deleteTime;		//!< Á–ÅŽžŠÔ

public:
	void Start();
	void Update();
};


// EOF