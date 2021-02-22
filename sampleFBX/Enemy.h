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
	EnemyState* m_state;		//!< 状態管理
	AI* m_ai;					//!< AI管理
	GameObject* m_LockIcon;		//!< ロックオンマーカー
	float m_defaultTime;		//!< デフォルトの計測時間(生成されてから削除までをカウント)
	
public:
	void Start();
	void Update();
	void Uninit();

	static void EnemyDelete(GameObject* obj);
};


// EOF