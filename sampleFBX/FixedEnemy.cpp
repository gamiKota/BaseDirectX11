/**
 * @file FixedEnemy.cpp
 */


/**
 * include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"
#include "Player.h"
#include "Collision.h"
#include "Transform.h"
#include "debugproc.h"
#include "System.h"


void FixedEnemy::Start() {
	Enemy::Start();
}


void FixedEnemy::Update() {
	GameObject* target = GameObject::Find("Player");
	if (target != nullptr) {
		m_transform->LookAt(target->m_transform);
	}
}


void FixedEnemy::OnCollision(GameObject* obj) {
	if (obj->ToString() == "Player") {	// �v���C���[�Ƃ̓����蔻��

		while (true) {	// �߂荞�񂾕����������߂�
			// �����Ƒ���̃x�N�g��
			float3 vec = obj->m_transform->m_position - m_transform->m_position;
			float scalar = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			vec /= scalar;

			// ���������炷
			m_transform->m_position -= vec * 1.f;

			// �K�v�ȏ��̍X�V����
			m_gameObject->GetComponent<Collision>()->LastUpdate();
			m_gameObject->GetComponent<Transform>()->LastUpdate();
			obj->GetComponent<Collision>()->LastUpdate();
			obj->GetComponent<Transform>()->LastUpdate();

			// ������Ȃ��Ȃ����烋�[�v����o��
			if (! Collision::OBB(*m_gameObject->GetComponent<Collision>(), *obj->GetComponent<Collision>())) {
				break;
			}
		}

	}
}


// EOF