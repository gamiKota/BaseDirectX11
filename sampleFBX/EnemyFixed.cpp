/**
 * @file FixedEnemy.cpp
 */


/**
 * @include
 */
#include "EnemyFixed.h"
#include "GameObject3D.h"
#include "PlayerMgr.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "TargetCtr.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "debugproc.h"
#include "System.h"


void EnemyFixed::Start() {
	// 敵の共通処理
	Enemy::Start();

	// 変数の初期化
	m_collider->m_weight = E_WEIGHT::_5;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// ステータス変数の初期化
	m_status->m_HP = 500.f;
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// 秒消滅
	m_status->m_bulletTime.max = 5.f;	// 弾発射間隔

	// 状態の初期化
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(1.f);
}


void EnemyFixed::Update() {
	// 敵の共通処理
	Enemy::Update();

	// ターゲット方向に向く処理
	// このコンポーネントはターゲットがプレイヤーから変わることはない
	//if (!m_state->GetCurrentState(ENEMY_STATE::TARGET_ON)) {
	//	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	//	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	//	m_state->GetState<EnemyState::TargetOn>()->SetAngle(1.f);
	//}

	// 射撃攻撃
	if (float3::Length(m_transform->m_position,
		m_state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position) < 2000.f) {
		if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
			m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
			m_status->m_bulletTime.InitData();
		}
	}
}


void EnemyFixed::OnCollision(GameObject* obj) {

}


// EOF