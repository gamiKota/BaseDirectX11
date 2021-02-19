/**
 * @file FixedEnemy.cpp
 */


/**
 * @include
 */
#include "EnemyFixed.h"
#include "GameObject3D.h"
#include "PlayerMgr.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "TargetCtr.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "debugproc.h"
#include "AI.h"
#include "PlayerState.h"
#include "System.h"


void Fixed_AI_Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
}
void Fixed_AI_Move(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
}
void Fixed_AI_Attack(AI* ai, EnemyState* state, Status* status) {
	state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	status->m_bulletTime.InitData();
}


void EnemyFixed::Start() {
	// �G�̋��ʏ���
	Enemy::Start();

	// �ϐ��̏�����
	m_collider->m_weight = E_WEIGHT::_5;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// �X�e�[�^�X�ϐ��̏�����
	m_status->SetHP(1000.f);
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// �b����
	m_status->m_bulletTime.max = 5.f;	// �e���ˊԊu

	// ��Ԃ̏�����
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(1.f);
}


void EnemyFixed::Update() {
	// �G�̋��ʏ���
	Enemy::Update();

	// �^�[�Q�b�g�����Ɍ�������
	// ���̃R���|�[�l���g�̓^�[�Q�b�g���v���C���[����ς�邱�Ƃ͂Ȃ�
	//if (!m_state->GetCurrentState(ENEMY_STATE::TARGET_ON)) {
	//	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	//	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	//	m_state->GetState<EnemyState::TargetOn>()->SetAngle(1.f);
	//}

	// �ˌ��U��
	if (float3::Length(m_transform->m_position,
		m_state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position) < 2000.f) {
		if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
			m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
			m_status->m_bulletTime.InitData();
		}
	}
}


void EnemyFixed::OnCollision(GameObject* obj) {

}


// EOF