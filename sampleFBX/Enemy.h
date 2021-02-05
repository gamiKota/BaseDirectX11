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


/**
 * @class Enemy : inheritance Character
 */
class Enemy : public Character {
protected:
	EnemyState* m_state;		//!< 状態管理
	GameObject* m_LockIcon;		//!< ロックオンマーカー
	GameObjectMesh* m_HPGauge;	//!< HPゲージ

public:
	void Start();
	void Update();
	void Uninit();
};


// EOF