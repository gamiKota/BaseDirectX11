/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "Status.h"
#include "Status.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "System.h"


void Enemy::Start() {
	// Characterクラスの初期化
	Character::Init();

	// 変数の初期化
	m_rigidbody->m_weight = E_WEIGHT::_1;
}


void Enemy::Update() {
	if (m_status->m_isDead) {
		Destroy(m_gameObject);
	}
}


// EOF