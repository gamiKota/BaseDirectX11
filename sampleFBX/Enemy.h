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
class AI;

/**
 * @class Enemy : inheritance Character
 */
class Enemy : public Character {
protected:
	EnemyState* m_state;		//!< ��ԊǗ�
	AI* m_ai;					//!< AI�Ǘ�
	GameObject* m_LockIcon;		//!< ���b�N�I���}�[�J�[
	float m_defaultTime;		//!< �f�t�H���g�̌v������(��������Ă���폜�܂ł��J�E���g)
	
public:
	void Start();
	void Update();
	void Uninit();

	static void EnemyDelete(GameObject* obj);
};


// EOF