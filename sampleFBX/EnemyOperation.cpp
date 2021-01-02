/*
 * @file EnemyOperation.cpp
 * このクラスなしで敵コンポーネントごとにAI追加か、
 * このクラスにある程度のAI情報を渡せるテーブルのどっちかがいい
 *
 * ボスなのどの敵は攻撃が派手だから特殊AIに見えるだけで雑魚敵と同じAIで回せるかも
 *
 * 操作クラスと敵クラスの基底から新しい敵を定義する

 Enemy
 継承クラス
  キャラクターの基底クラス
  コンポーネント
 メンバ関数
  オーバーライド関数(ゲームオブジェクトに対して操作される関数)
 メンバ変数
  

 * そもそも弾を打つなのどの操作変数はステータスが持つべきなのか
 * ステータスの変数値を自分のものに変えて、独自のAIに基づき状態管理クラスに状態依頼を送るのが
 * Enemyから継承された各敵ユニットがよさそう
 * 状態管理クラスはあくまでも状態のふるまいを定義(状態のふるまいに関する変数も持つ)
 * ステータスは攻撃力や体力、弾のクールタイムに消滅時間などのキャラクターそのものに関する値を持つ


 */


/**
 * @include
 */
#include "EnemyOperation.h"
#include "Component.h"
#include "GameObject.h"
#include "EnemyState.h"
#include "Frame.h"
#include "debugproc.h"
#include "System.h"


void EnemyOperation::Awake() {
	// 変数の初期化(デフォルト値)
	//m_bullet = StatusVal(0.f, 0.f, 3.f);	// 3秒ごとに弾を発射
	//m_targetDist = StatusVal(0.f, 1500.f, 2000.f);
	//m_movement = float3(0.5f, 0.5f, 0.5f);
}


void EnemyOperation::Start() {
	//m_stateMachine = m_gameObject->GetComponent<EnemyState>();
}


void EnemyOperation::Update() {
	// ターゲット処理
	// プレイヤーなのかプレイヤーの防衛オブジェクトなのかの判断はここでする

	//if (GameObject::Find("Player") != nullptr) {	// ロックオン
	//	m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_ON,  true);
	//	m_stateMachine->SetTarget(GameObject::Find("Player"));
	//}
	//else {		// ロックオフ
	//	m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_OFF, true);
	//}
	//
	//// ターゲットがない
	//if (m_stateMachine->GetTarget() == nullptr) {
	//	return;
	//}
	//
	//// 移動処理
	//// 距離をとって遠距離攻撃するか近接攻撃をするかはここで判断する
	//float3 movement = float3();
	//m_targetDist.data = float3::Length(m_stateMachine->GetTarget()->m_transform->m_position, m_transform->m_position);
	//if (m_targetDist.data < m_targetDist.min) {
	//	movement.z += -m_movement.z;
	//}
	//else if (m_targetDist.data >= m_targetDist.max) {
	//	movement.z += m_movement.z;
	//}
	//else {
	//	movement.x += m_movement.x;
	//	m_bullet.data += Frame::GetInstance().GetDeltaTime();
	//}
	//if (movement.x == 0.f && movement.y == 0.f && movement.z == 0.f) {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::IDOL, true);
	//}
	//else {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::MOVE, true);
	//	m_stateMachine->SetMovement(movement);
	//}
	//
	//// 射撃攻撃
	//if (m_bullet.data >= m_bullet.max) {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_bullet.data = m_bullet.min;
	//}
}


// EOF