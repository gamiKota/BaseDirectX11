#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "System.h"


/*
AI
�߂Â��ꂽ�痣���
����ȊO�Ȃ�s����ɒe������
�����_�����b���ƂɃ����_���Ɉړ�(X���AY���A����ɗ����ړ�)
*/

void Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
	// �����������_�����ɂ���(��~����)
}
void Move(AI* ai, EnemyState* state) {
	float len = float3::Length(
		state->m_transform->m_position, state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position
	);
	// �v���C���[�ɑ΂���s��
	if (state->GetState<EnemyState::TargetOn>()->GetTarget() == GameObject::FindGameObjectWithTag("Player")) {
		if (len <= 1000.f) {	// �߂��ꍇ
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.5f);
		}
		else {	// �\���ȋ������Ƃ�Ă�ꍇ
			// �����������_�����ɂ���(�ړ�����)
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.4f, 0.f, 0.f);
		}
	}
}
void Attack(AI* ai, EnemyState* state, Status* status) {
	if (status->m_bulletTime.data >= status->m_bulletTime.max) {
		state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		status->m_bulletTime.InitData();
	}
}


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
	//m_state->GetState<EnemyState::Move>()->m_movement.x = 1.f;
	// �U���Ώ�
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(2.f);

	// AI�e�[�u��
	m_ai->m_table.push_back([sub = this] { Idol(sub->m_ai, sub->m_state); });
	m_ai->m_table.push_back([sub = this] { Move(sub->m_ai, sub->m_state); });
	m_ai->m_table.push_back([sub = this] { });
	m_ai->m_table.push_back([sub = this] { });
	m_ai->m_table.push_back([sub = this] { Attack(sub->m_ai, sub->m_state, sub->m_status); });
}

void EnemyNormal::Update() {
	// �G�̋��ʏ���
	Enemy::Update();

	// �ˌ�
	m_ai->m_table[(int)ENEMY_STATE::ATTACK_BULLET]();
	// �ړ�
	m_ai->m_table[(int)ENEMY_STATE::MOVE]();
}

// EOF