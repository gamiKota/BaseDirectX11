/**
 * @file FixedEnemy.cpp
 */


/**
 * @include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"
#include "PlayerMgr.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "TargetCtr.h"
#include "EnemyState.h"
#include "debugproc.h"
#include "System.h"


void FixedEnemy::Start() {
	// 敵の共通処理
	Enemy::Start();

	// ステータス変数の初期化
	m_status->m_HP = 500.f;
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// 秒消滅
	m_status->m_bulletTime.max = 5.f;	// 弾発射間隔

	// 状態の初期化
	//m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	//m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	//m_state->GetState<EnemyState::TargetOn>()->SetAngle(1.f);
}


void FixedEnemy::Update() {
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
	//if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
	//	m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_status->m_bulletTime.InitData();
	//}
}


void FixedEnemy::OnCollision(GameObject* obj) {

}


// EOF