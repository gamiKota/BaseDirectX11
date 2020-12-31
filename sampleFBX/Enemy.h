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
class EnemyOperation;


/**
 * @class Enemy : inheritance Character
 */
class Enemy : public Character {
private:
	float m_deleteTime;			//!< 消滅時間
	EnemyState* m_state;		//!< 状態管理
	EnemyOperation* m_operate;	//!< 操作管理

protected:
	GameObject* m_LockIcon;	//!< ロックオンマーカー

public:
	void Start();
	void Update();
	void Uninit();
};


// EOF