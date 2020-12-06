/**
 * @file Character.h
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
 * @forward declaration
 */
class Status;
class Collision;
class Rigidbody;


/**
 * @class Character : inheritance Component
 */
class Character : public Component {
public:
	void Init();

public:
	Status*		m_status;		//!< ステータス
	Collision*	m_collision;	//!< 当たり判定
	Rigidbody*	m_rigidbody;	//!< 物理エンジン機能
};


// EOF