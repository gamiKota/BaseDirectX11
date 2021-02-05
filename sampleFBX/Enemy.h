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


class EnemyState;
class GameObjectUI;

/**
 * @class Enemy : inheritance Character
 */
class Enemy : public Character {
protected:
	EnemyState* m_state;		//!< ��ԊǗ�
	GameObject* m_LockIcon;		//!< ���b�N�I���}�[�J�[

public:
	void Start();
	void Update();
	void Uninit();
};


// EOF