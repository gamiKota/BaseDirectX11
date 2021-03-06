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
#include "AI.h"
#include "PlayerState.h"
#include "System.h"


enum class FixedEnemyAI {
	Attack,
	Delete,
};

AI* Fixed_AI_Attack(AI* ai, EnemyState* state, Status* status) {
	if (status->m_bulletTime.data >= status->m_bulletTime.max) {
		state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		status->m_bulletTime.InitData();
	}
	return ai;
}
AI* Fixed_AI_Delete(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::DEFEATED, true);
	state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.2f);
	ai->StartUp(2.f, true)->OnComplete([obj = ai->m_gameObject]{ Enemy::EnemyDelete(obj); });
	return ai;
}


void EnemyFixed::Start() {
	// 敵の共通処理
	Enemy::Start();

	// 変数の初期化
	m_collider->m_weight = E_WEIGHT::_1;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// ステータス変数の初期化
	m_status->SetHP(1000.f);
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// 秒消滅
	m_status->m_bulletTime.max = 5.f;	// 弾発射間隔

	// 状態の初期化
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(1.f);

	// AIテーブル
	m_ai->m_table[(int)FixedEnemyAI::Attack] = [sub = this] { return Fixed_AI_Attack(sub->m_ai, sub->m_state, sub->m_status); };
	m_ai->m_table[(int)FixedEnemyAI::Delete] = [sub = this] { return Fixed_AI_Delete(sub->m_ai, sub->m_state); };
}


void EnemyFixed::Update() {
	// 敵の共通処理
	Enemy::Update();

	// 死亡時
	if (m_status->m_isDead) {
		m_ai->m_table[(int)FixedEnemyAI::Delete]();
	}
	// 生存時
	else {
		//--- 射撃
		//m_ai->m_table[(int)FixedEnemyAI::Attack]();
	}
}


void EnemyFixed::OnCollision(GameObject* obj) {

}


// EOF