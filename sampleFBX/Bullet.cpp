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
#include "Frame.h"
#include "System.h"


/**
 * @constant
 */
static const float	SPEED		= 50.0f;	// 速さ
static const char	LIFE_TIME	= 5;		// 生存時間


void Bullet::Start() {
	m_gameObject->AddComponent<Collision>();
	m_nLife = LIFE_TIME * Frame::GetInstance().GetFrame();	// 5秒
}


void Bullet::Update() {
	--m_nLife;
	if (m_nLife <= 0) {
		Destroy(m_gameObject);
		return;
	}
	// 前に進む
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	if (obj->GetTag() == "Enemy") {
		//Destroy(obj);
		Destroy(m_gameObject);
		obj->GetComponent<Collision>()->m_bHit = true;
		if (GameObject::Find("Score") != nullptr) {
			GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
		}
		//if (GameObject::Find("Player") != nullptr) {
		//	if (obj == GameObject::Find("Player")->GetComponent<Player>()->m_target)
		//		GameObject::Find("Player")->GetComponent<Player>()->m_target = nullptr;
		//}
	}
}


// EOF