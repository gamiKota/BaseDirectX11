/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "Status.h"
#include "PlayerState.h"
#include "EnemyState.h"
#include "EnemyOperation.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "TargetCtr.h"
#include "GameObjectMesh.h"
#include "HPGauge.h"
#include "Frame.h"
#include "System.h"


void Enemy::Start() {
	// Characterクラスの初期化
	Character::Init();

	// 敵AIに必要なコンポーネントの追加
	// 操作は各敵継承コンポーネントで行う
	m_state = m_gameObject->AddComponent<EnemyState>();
	//m_operate = m_gameObject->AddComponent<EnemyOperation>();

	// 変数の初期化(敵クラスのデフォルト値の設定)
	m_collider->m_weight = E_WEIGHT::_2;	// 二番目に軽い

	// ロックオンマーカーの追加(とりあえず今は表示しない)
	GameObjectUI* obj = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN, "EnemyIcon");
	obj->AddComponent<TargetCtr>()->m_target = m_gameObject;
	m_LockIcon = GameObject::Instantiate(obj);

	// HPゲージ
	m_HPGauge = new GameObjectMesh(E_MESH_TYPE::NORMAL, E_TEXTURE::E_TEXTURE_TREE, "HPGauge", "HPGauge");
	m_HPGauge->AddComponent<HPGauge>()->m_pair = m_gameObject;
	GameObject::Instantiate(m_HPGauge, float3(), Quaternion::identity, float3(100.f, 20.f, 0.f));
}


void Enemy::Uninit() {
	Destroy(m_LockIcon);
	Destroy(m_HPGauge);
}


void Enemy::Update() {
	if (m_status->m_isDead) {
		if (GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget() != nullptr &&
			m_gameObject->GetInstanceID() == 
			GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget()->GetInstanceID()) {	// 今自分がターゲットの場合
			GameObject::Find("Player")->GetComponent<PlayerState>()->SetTarget();
		}
		Destroy(m_gameObject);
	}
}


// EOF