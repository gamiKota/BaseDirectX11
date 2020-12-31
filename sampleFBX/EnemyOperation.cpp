/*
 * @file EnemyOperation.cpp
 */


/**
 * @include
 */
#include "EnemyOperation.h"
#include "Component.h"
#include "GameObject.h"
#include "EnemyState.h"
#include "Frame.h"
#include "debugproc.h"
#include "System.h"



float getDistance(float3 pos1, float3 pos2) {
	float distance = 
		sqrtf((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z));

	return distance;
}


void EnemyOperation::Awake() {
	// �ϐ��̏�����(�f�t�H���g�l)
	m_bullet = StatusVal(0.f, 0.f, 3.f);	// 3�b���Ƃɒe�𔭎�
	m_targetDist = StatusVal(0.f, 1500.f, 2000.f);
	m_movement = float3(0.5f, 0.5f, 0.5f);
}


void EnemyOperation::Start() {
	m_stateMachine = m_gameObject->GetComponent<EnemyState>();
}


void EnemyOperation::Update() {
	// �^�[�Q�b�g����
	// �v���C���[�Ȃ̂��v���C���[�̖h�q�I�u�W�F�N�g�Ȃ̂��̔��f�͂����ł���

	if (GameObject::Find("Player") != nullptr) {	// ���b�N�I��
		m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_ON,  true);
		m_stateMachine->SetTarget(GameObject::Find("Player"));
	}
	else {		// ���b�N�I�t
		m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_OFF, true);
	}

	// �^�[�Q�b�g���Ȃ�
	if (m_stateMachine->GetTarget() == nullptr) {
		return;
	}

	// �ړ�����
	// �������Ƃ��ĉ������U�����邩�ߐڍU�������邩�͂����Ŕ��f����
	float3 movement = float3();
	m_targetDist.data = getDistance(m_stateMachine->GetTarget()->m_transform->m_position, m_transform->m_position);
	if (m_targetDist.data < m_targetDist.min) {
		movement.z += -m_movement.z;
	}
	else if (m_targetDist.data >= m_targetDist.max) {
		movement.z += m_movement.z;
	}
	else {
		movement.x += m_movement.x;
		m_bullet.data += Frame::GetInstance().GetDeltaTime();
	}
	if (movement.x == 0.f && movement.y == 0.f && movement.z == 0.f) {
		m_stateMachine->SetStateActive(ENEMY_STATE::IDOL, true);
	}
	else {
		m_stateMachine->SetStateActive(ENEMY_STATE::MOVE, true);
		m_stateMachine->SetMovement(movement);
	}

	// �ˌ��U��
	if (m_bullet.data >= m_bullet.max) {
		m_stateMachine->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
		m_bullet.data = m_bullet.min;
	}
}


// EOF