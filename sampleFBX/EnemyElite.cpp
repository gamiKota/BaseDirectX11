#include "EnemyElite.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "PlayerState.h"
#include "System.h"


/*
AI
近づかれたら離れる
それ以外なら不定期に弾うって
ランダム数秒ごとにランダムに移動(X軸、Y軸、さらに離れる移動)
*/


enum class EliteEnemyAI {
	Idol,
	MoveBack,
	MoveFront,
	MoveSide,
	Attack,
	Delete,
};


// 停止時の行動
AI* Elite_AI_Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
	// ここをランダム性にする(停止時間)
	return ai;
}
// 動作時の行動
AI* Elite_AI_Move_Back(AI* ai, EnemyState* state) {
	float len = float3::Length(state->m_transform->m_position, state->GetTarget()->m_transform->m_position);
	GameObject* player = GameObject::FindGameObjectWithTag("Player");
	// プレイヤーに対する行動
	if (state->GetState<EnemyState::TargetOn>()->GetTarget() == player) {
		if (len <= 1000.f) {	// 近い場合
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.5f);
		}
	}
	// プレイヤー以外への行動
	else {

	}
	return ai;
}
// 攻撃時の行動
AI* Elite_AI_Attack(AI* ai, EnemyState* state, Status* status) {
	if (status->m_bulletTime.data >= status->m_bulletTime.max) {
		state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		status->m_bulletTime.InitData();
	}
	return ai;
}
// 被撃破時の行動
AI* Elite_AI_Delete(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::DEFEATED , true);
	state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.2f);
	ai->StartUp(5.f, true)->OnComplete([obj = ai->m_gameObject] { Enemy::EnemyDelete(obj); });
	return ai;
}


void EnemyElite::Start() {
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
	// 攻撃対象
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(2.f);

	// AIテーブル
	m_ai->m_table[(int)EliteEnemyAI::Idol] = [sub = this] { return Elite_AI_Idol(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)EliteEnemyAI::MoveBack] = [sub = this] { return Elite_AI_Move_Back(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)EliteEnemyAI::Attack] = [sub = this] { return Elite_AI_Attack(sub->m_ai, sub->m_state, sub->m_status); };
	m_ai->m_table[(int)EliteEnemyAI::Delete] = [sub = this] { return Elite_AI_Delete(sub->m_ai, sub->m_state); };
}

void EnemyElite::Update() {
	// 敵の共通処理
	Enemy::Update();

	// 死亡時
	if (m_status->m_isDead) {
		m_ai->m_table[(int)EliteEnemyAI::Delete]();
	}
	// 生存時
	else {
		//--- 射撃
		m_ai->m_table[(int)EliteEnemyAI::Attack]();
		//--- 移動
		// ここでランダム性やイベント発行を出す
		// 例) HPが下がったら移動早くする、もしくはAIテーブル自体の変更
		m_ai->m_table[(int)EliteEnemyAI::MoveBack]();
	}
}


// EOF