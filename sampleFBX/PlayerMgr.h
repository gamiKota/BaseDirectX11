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


/**
 * @class PlayerMgr : inheritance Character
 */
class PlayerMgr : public Character
{
public:

public:
	void Start();
	void Update();
	void Uninit();
	void OnCollision(GameObject* obj);
	void SetImGuiVal();
};


// EOF