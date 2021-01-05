/**
 * @file FixedEnemy.cpp
 */


/**
 * @include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"
#include "PlayerMgr.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "TargetCtr.h"
#include "EnemyState.h"
#include "debugproc.h"
#include "System.h"


void FixedEnemy::Start() {
	// �G�̋��ʏ���
	Enemy::Start();

	// �X�e�[�^�X�ϐ��̏�����
	m_status->m_HP = 500.f;
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// �b����
	m_status->m_bulletTime.max = 5.f;	// �e���ˊԊu

	// ��Ԃ̏�����
	//m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	//m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	//m_state->GetState<EnemyState::TargetOn>()->SetAngle(1.f);
}


void FixedEnemy::Update() {
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
	//if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
	//	m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_status->m_bulletTime.InitData();
	//}
}


void FixedEnemy::OnCollision(GameObject* obj) {

}


// EOF