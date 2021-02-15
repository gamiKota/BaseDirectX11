#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "Frame.h"
#include "System.h"

void EnemyNormal::Start() {
	// 敵の共通処理
	Enemy::Start();

	// 変数の初期化
	m_collider->m_weight = E_WEIGHT::_2;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// ステータス変数の初期化
	m_status->SetHP(1000.f);
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// 秒消滅
	m_status->m_bulletTime.max = 3.f;	// 弾発射間隔

	// 状態の初期化
	// 移動
	m_state->SetStateActive(ENEMY_STATE::MOVE, true);
	//m_state->GetState<EnemyState::Move>()->m_movement.x = 1.f;
	// 攻撃対象
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(5.f);
}

void EnemyNormal::Update() {
	// 敵の共通処理
	Enemy::Update();

	float len = float3::Length(m_transform->m_position, m_state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position);

	//// 移動
	//if (len <= 1500.f) {
	//	m_state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, 1.f);
	//}
	//if (len)

	//// 射撃攻撃
	//if (len < 2000.f) {
	//	if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
	//		m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//		m_status->m_bulletTime.InitData();
	//	}
	//}
}

// EOF