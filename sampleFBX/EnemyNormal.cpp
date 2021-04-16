#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "PlayerState.h"
#include "System.h"


/*
AI
�߂Â��ꂽ�痣���
����ȊO�Ȃ�s����ɒe������
�����_�����b���ƂɃ����_���Ɉړ�(X���AY���A����ɗ����ړ�)
*/


enum class NoramalEnemyAI {
	Idol,
	Move,
	Attack,
	Delete,
};


// ��~���̍s��
AI* Normal_AI_Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
	return ai;
	// �����������_�����ɂ���(��~����)
}
// ���쎞�̍s��
AI* Normal_AI_Move(AI* ai, EnemyState* state) {
	float len = float3::Length(state->m_transform->m_position, state->GetTarget()->m_transform->m_position);
	GameObject* player = GameObject::FindGameObjectWithTag("Player");
	// �v���C���[�ɑ΂���s��
	if (state->GetState<EnemyState::TargetOn>()->GetTarget() == player) {
		if (len <= 1000.f) {	// �߂��ꍇ
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.5f);
		}
		else {	// �\���ȋ������Ƃ�Ă�ꍇ
			int move = rand() % 2;
			if (player->GetComponent<PlayerState>()->GetTarget() == ai->m_gameObject) {
				ai->StartUp(3.f, true)->OnStart([s = state, _move = move] {
					s->SetStateActive(ENEMY_STATE::MOVE, true);
					s->GetState<EnemyState::Move>()->m_movement = float3(_move == 0 ? -0.4f : 0.4f, 0.f, 0.f);
				});
			}
			else {
				ai->StartUp(5.f, true)->OnStart([s = state, _move = move] {
					int move = rand() % 2;
					s->SetStateActive(ENEMY_STATE::MOVE, true);
					s->GetState<EnemyState::Move>()->m_movement = float3(_move == 0 ? -0.2f : 0.2f, 0.f, 0.f);
				});
			}
		}
	}
	return ai;
}
// �U�����̍s��
AI* Normal_AI_Attack(AI* ai, EnemyState* state, Status* status) {
	if (status->m_bulletTime.data >= status->m_bulletTime.max) {
		state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		status->m_bulletTime.InitData();
	}
	return ai;
}
// �U�����̍s��
AI* Normal_AI_Delete(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::DEFEATED, true);
	state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.2f);
	ai->StartUp(2.f, true)->OnComplete([obj = ai->m_gameObject]{ Enemy::EnemyDelete(obj); });
	return ai;
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
	// �U���Ώ�
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(2.f);

	// AI�e�[�u��(�����ƍs�����[�`���𕪂��Ă�������������Ȃ�)
	m_ai->m_table[(int)NoramalEnemyAI::Idol] = [sub = this] { return Normal_AI_Idol(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)NoramalEnemyAI::Move] = [sub = this] { return Normal_AI_Move(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)NoramalEnemyAI::Attack] = [sub = this] { return Normal_AI_Attack(sub->m_ai, sub->m_state, sub->m_status); };
	m_ai->m_table[(int)NoramalEnemyAI::Delete] = [sub = this] { return Normal_AI_Delete(sub->m_ai, sub->m_state); };
}

void EnemyNormal::Update() {
	// �G�̋��ʏ���
	Enemy::Update();

	// ���S��
	if (m_status->m_isDead) {
		m_ai->m_table[(int)NoramalEnemyAI::Delete]();
	}
	// ������
	else {
		//--- �ˌ�
		m_ai->m_table[(int)NoramalEnemyAI::Attack]();
		//	->OnComplete(
		//	[sub = this]
		//	{
		//		sub->Start();
		//	}
		//);
		//--- �ړ�
		// �����Ń����_������C�x���g���s���o��
		// ��) HP������������ړ���������A��������AI�e�[�u�����̂̕ύX
		m_ai->m_table[(int)NoramalEnemyAI::Move]();
	}
}

// EOF