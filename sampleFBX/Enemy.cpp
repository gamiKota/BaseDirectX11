/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "Status.h"
#include "Status.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "Frame.h"
#include "System.h"


void Enemy::Start() {
	// Character�N���X�̏�����
	Character::Init();

	// �ϐ��̏�����(�G�N���X�̃f�t�H���g�l�̐ݒ�)
	m_deleteTime = 3.f;						// �b��
	m_rigidbody->m_weight = E_WEIGHT::_1;	// ��ԖڂɌy��
}


void Enemy::Update() {
	if (m_status->m_isDead) {
		if (m_deleteTime <= 0.f) {
			Destroy(m_gameObject);
		}
		m_deleteTime -= Frame::GetInstance().GetDeltaTime();
	}
}


// EOF