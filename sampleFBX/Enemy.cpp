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
#include "Frame.h"
#include "System.h"


void Enemy::Start() {
	// Characterクラスの初期化
	Character::Init();

	// 敵AIに必要なコンポーネントの追加
	m_state = m_gameObject->AddComponent<EnemyState>();
	m_operate = m_gameObject->AddComponent<EnemyOperation>();

	// 変数の初期化(敵クラスのデフォルト値の設定)
	m_deleteTime = 3.f;						// 秒数
	m_rigidbody->m_weight = E_WEIGHT::_1;	// 二番目に軽い

	// ロックオンマーカーの追加
	GameObjectUI* obj = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN, "EnemyIcon");
	obj->AddComponent<TargetCtr>()->m_target = m_gameObject;
	m_LockIcon = GameObject::Instantiate(obj);
}


void Enemy::Uninit() {
	Destroy(m_LockIcon);
}


void Enemy::Update() {
	if (m_status->m_isDead) {
		if (m_deleteTime <= 0.f) {
			if (m_gameObject->GetInstanceID() == 
				GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget()->GetInstanceID()) {	// 今自分がターゲットの場合
				GameObject::Find("Player")->GetComponent<PlayerState>()->SetTarget();
			}
			Destroy(m_gameObject);
		}
		m_deleteTime -= Frame::GetInstance().GetDeltaTime();
	}
}


// EOF