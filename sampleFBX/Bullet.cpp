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
#include "Status.h"
#include "Player.h"
#include "Frame.h"
#include "Rigidbody.h"
#include "System.h"


/**
 * @constant
 */
static const float	SPEED		= 50.0f;	// ����
static const char	LIFE_TIME	= 5;		// ��������


void Bullet::Start() {
	// Character�N���X�̏�����
	Character::Init();

	// ������
	m_nLife = LIFE_TIME * Frame::GetInstance().GetFrame();	// 5�b
	m_status->m_HP = 10.f;
	m_status->m_AttakPower = 100.f;
	m_status->m_HitStop = 5.f * Frame::GetInstance().GetFrame();
	m_rigidbody->m_weight = E_WEIGHT::_0;
}


void Bullet::Update() {
	--m_nLife;
	if (m_nLife <= 0) {
		Destroy(m_gameObject);
		return;
	}
	// �O�ɐi��
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	if (obj->GetTag() == m_gameObject->GetTag() ||	// �����^�O�Ȃ�Փ˔���Ȃ�
		obj->GetTag() == "AreaWall"					// �G���A�O
		) {
		return;
	}
	Destroy(m_gameObject);
	if (obj->GetComponent<Status>() != nullptr) {
		obj->GetComponent<Status>()->m_HP -= m_status->m_AttakPower;
		obj->GetComponent<Status>()->m_HitStop -= m_status->m_HitStop;
	}
	if (GameObject::Find("Score") != nullptr) {
		GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
	}
}


// EOF