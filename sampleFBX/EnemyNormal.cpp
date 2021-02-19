#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "System.h"


/*
AI
近づかれたら離れる
それ以外なら不定期に弾うって
ランダム数秒ごとにランダムに移動(X軸、Y軸、さらに離れる移動)
*/

void Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
	// ここをランダム性にする(停止時間)
}
void Move(AI* ai, EnemyState* state) {
	float len = float3::Length(
		state->m_transform->m_position, state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position
	);
	// プレイヤーに対する行動
	if (state->GetState<EnemyState::TargetOn>()->GetTarget() == GameObject::FindGameObjectWithTag("Player")) {
		if (len <= 1000.f) {	// 近い場合
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.5f);
		}
		else {	// 十分な距離がとれてる場合
			// ここをランダム性にする(移動方向)
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.4f, 0.f, 0.f);
		}
	}
}
void Attack(AI* ai, EnemyState* state, Status* status) {
	if (status->m_bulletTime.data >= status->m_bulletTime.max) {
		state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		status->m_bulletTime.InitData();
	}
}


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
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(2.f);

	// AIテーブル
	m_ai->m_table.push_back([sub = this] { Idol(sub->m_ai, sub->m_state); });
	m_ai->m_table.push_back([sub = this] { Move(sub->m_ai, sub->m_state); });
	m_ai->m_table.push_back([sub = this] { });
	m_ai->m_table.push_back([sub = this] { });
	m_ai->m_table.push_back([sub = this] { Attack(sub->m_ai, sub->m_state, sub->m_status); });
}

void EnemyNormal::Update() {
	// 敵の共通処理
	Enemy::Update();

	// 射撃
	m_ai->m_table[(int)ENEMY_STATE::ATTACK_BULLET]();
	// 移動
	m_ai->m_table[(int)ENEMY_STATE::MOVE]();
}

// EOF