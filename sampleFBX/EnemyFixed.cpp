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
#include "debugproc.h"
#include "System.h"


 // 対象オブジェクト
GameObject* gameObject;


void EnemyFixed::Start() {
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

	gameObject = GameObject::FindGameObjectWithTag("Player");
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
	//if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
	//	m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_status->m_bulletTime.InitData();
	//}


	// 補完スピードを決める
	//float speed = 0.01f;
	//// ターゲット方向のベクトルを取得
	//float3 relativePos = gameObject->m_transform->m_position - m_transform->m_position;
	//// 方向を、回転情報に変換
	//Quaternion q1 = Quaternion::Normalize(m_transform->m_rotation);
	//Quaternion q2 = Quaternion::LookRotation(relativePos);
	// 現在の回転情報と、ターゲット方向の回転情報を補完する
	//m_transform->m_rotation = Quaternion::Slerp(q1, q2, speed);
	//m_transform->m_rotation = q2;
	//m_transform->m_rotation = Quaternion::RotateTowards(m_transform->m_rotation, rotation, speed);
	m_transform->LookAt(gameObject->m_transform);
}


void EnemyFixed::OnCollision(GameObject* obj) {

}


// EOF