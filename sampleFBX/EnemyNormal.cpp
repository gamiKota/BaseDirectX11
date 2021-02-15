#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "System.h"

void EnemyNormal::Start() {
	// �G�̋��ʏ���
	Enemy::Start();

	// �ϐ��̏�����
	m_collider->m_weight = E_WEIGHT::_2;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// �X�e�[�^�X�ϐ��̏�����
	m_status->SetHP(1000.f);
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// �b����
	m_status->m_bulletTime.max = 3.f;	// �e���ˊԊu

	// ��Ԃ̏�����
	// �ړ�
	m_state->SetStateActive(ENEMY_STATE::MOVE, true);
	m_state->GetState<EnemyState::Move>()->m_movement.x = 1.f;
	// �U���Ώ�
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(5.f);
}

void EnemyNormal::Update() {
	// �G�̋��ʏ���
	Enemy::Update();

	// �ړ�

	// �ˌ��U��
	if (float3::Length(m_transform->m_position,
		m_state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position) < 2000.f) {
		if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
			m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
			m_status->m_bulletTime.InitData();
		}
	}
}

// EOF