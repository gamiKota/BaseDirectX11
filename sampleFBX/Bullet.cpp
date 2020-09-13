/**
 * @file Bullet.cpp
 */


/**
 * @include
 */
#include "Bullet.h"
#include "GameObject.h"
#include "Collision.h"
#include "Score.h"
#include "Player.h"
#include "System.h"


/**
 * @constant
 */
static const float	SPEED		= 50.0f;	// 速さ
static const char	LIFE_TIME	= 5;		// 生存時間


void Bullet::Start() {
	m_nLife = 0;
	m_nStat = 0;
}


void Bullet::Update() {
	// 未使用?
	if (m_nStat == 0) {
		return;
	}
	// 追尾中 (m_nStat == 1)
	--m_nLife;
	if (m_nLife <= 0) {
		// 爆発開始
		m_nLife = 3 * 60;
		m_nStat = 0;
		GameObject::Destroy(m_gameObject);
		return;
	}
	// 前に進む
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	if (obj->GetTag() == "Enemy") {
		//GameObject::Destroy(obj);
		//Destroy(m_gameObject);
		obj->GetComponent<Collision>()->m_bHit = true;
		if (GameObject::Find("Score") != nullptr) {
			GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
		}
		//if (GameObject::Find("Player") != nullptr) {
		//	if (obj == GameObject::Find("Player")->GetComponent<CPlayer>()->m_target)
		//		GameObject::Find("Player")->GetComponent<CPlayer>()->m_target = nullptr;
		//}
	}
}


void Bullet::Fire(Transform transform) {

	m_transform->m_position = transform.m_position;
	m_transform->m_rotate = transform.m_rotate;
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };

	m_nLife = LIFE_TIME * 60;	// 5秒
	m_nStat = 1;				// 追尾中
}


// EOF