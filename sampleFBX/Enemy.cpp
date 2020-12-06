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
#include "Frame.h"
#include "System.h"


void Enemy::Start() {
	// Characterクラスの初期化
	Character::Init();

	// 変数の初期化(敵クラスのデフォルト値の設定)
	m_deleteTime = 3.f;						// 秒数
	m_rigidbody->m_weight = E_WEIGHT::_1;	// 二番目に軽い
}


void Enemy::Update() {
	if (m_status->m_isDead) {
		if (m_deleteTime <= 0.f) {
			Destroy(m_gameObject);
		}
		m_deleteTime -= Frame::GetInstance().GetDeltaTime();
	}
}


// EOF