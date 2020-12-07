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
	float m_deleteTime;		//!< ���Ŏ���

protected:
	GameObject* m_LockIcon;	//!< ���b�N�I���}�[�J�[

public:
	void Start();
	void Update();
	void Uninit();
};


// EOF