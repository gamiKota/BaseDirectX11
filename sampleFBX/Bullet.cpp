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
static const float	SPEED		= 50.0f;	// ����
static const char	LIFE_TIME	= 5;		// ��������


void Bullet::Start() {
	m_nLife = 0;
	m_nStat = 0;
}


void Bullet::Update() {
	// ���g�p?
	if (m_nStat == 0) {
		return;
	}
	// �ǔ��� (m_nStat == 1)
	--m_nLife;
	if (m_nLife <= 0) {
		// �����J�n
		m_nLife = 3 * 60;
		m_nStat = 0;
		GameObject::Destroy(m_gameObject);
		return;
	}
	// �O�ɐi��
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

	m_nLife = LIFE_TIME * 60;	// 5�b
	m_nStat = 1;				// �ǔ���
}


// EOF