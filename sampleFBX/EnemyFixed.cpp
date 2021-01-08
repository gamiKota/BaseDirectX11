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
#include "debugproc.h"
#include "System.h"


 // �ΏۃI�u�W�F�N�g
GameObject* gameObject;


void EnemyFixed::Start() {
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

	gameObject = GameObject::FindGameObjectWithTag("Player");
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
	//if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
	//	m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_status->m_bulletTime.InitData();
	//}


	// �⊮�X�s�[�h�����߂�
	//float speed = 0.01f;
	//// �^�[�Q�b�g�����̃x�N�g�����擾
	//float3 relativePos = gameObject->m_transform->m_position - m_transform->m_position;
	//// �������A��]���ɕϊ�
	//Quaternion q1 = Quaternion::Normalize(m_transform->m_rotation);
	//Quaternion q2 = Quaternion::LookRotation(relativePos);
	// ���݂̉�]���ƁA�^�[�Q�b�g�����̉�]����⊮����
	//m_transform->m_rotation = Quaternion::Slerp(q1, q2, speed);
	//m_transform->m_rotation = q2;
	//m_transform->m_rotation = Quaternion::RotateTowards(m_transform->m_rotation, rotation, speed);
	m_transform->LookAt(gameObject->m_transform);
}


void EnemyFixed::OnCollision(GameObject* obj) {

}


// EOF