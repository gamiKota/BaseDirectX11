/**
 * @file Status.h
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
 * @class Status : inheritance Component
 * @brief 各オブジェクト(ミサイルなど)に持たせる
 */
class Status : public Component {
public:	// 自分が持つステータス
	float	m_HP;			//!< 体力
	float	m_AttakPower;	//!< 攻撃力
	float	m_HitStop;		//!< 被弾時間
	bool	m_isDead;		//!< 死亡フラグ

public:

public:
	Status();
	void Start();
	void Update();
	void SetImGuiVal();
};


// EOF