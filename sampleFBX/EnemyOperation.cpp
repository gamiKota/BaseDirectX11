/*
 * @file EnemyOperation.cpp
 * ���̃N���X�Ȃ��œG�R���|�[�l���g���Ƃ�AI�ǉ����A
 * ���̃N���X�ɂ�����x��AI����n����e�[�u���̂ǂ�����������
 *
 * �{�X�Ȃ̂ǂ̓G�͍U�����h�肾�������AI�Ɍ����邾���ŎG���G�Ɠ���AI�ŉ񂹂邩��
 *
 * ����N���X�ƓG�N���X�̊�ꂩ��V�����G���`����

 Enemy
 �p���N���X
  �L�����N�^�[�̊��N���X
  �R���|�[�l���g
 �����o�֐�
  �I�[�o�[���C�h�֐�(�Q�[���I�u�W�F�N�g�ɑ΂��đ��삳���֐�)
 �����o�ϐ�
  

 * ���������e��łȂ̂ǂ̑���ϐ��̓X�e�[�^�X�����ׂ��Ȃ̂�
 * �X�e�[�^�X�̕ϐ��l�������̂��̂ɕς��āA�Ǝ���AI�Ɋ�Â���ԊǗ��N���X�ɏ�Ԉ˗��𑗂�̂�
 * Enemy����p�����ꂽ�e�G���j�b�g���悳����
 * ��ԊǗ��N���X�͂����܂ł���Ԃ̂ӂ�܂����`(��Ԃ̂ӂ�܂��Ɋւ���ϐ�������)
 * �X�e�[�^�X�͍U���͂�̗́A�e�̃N�[���^�C���ɏ��Ŏ��ԂȂǂ̃L�����N�^�[���̂��̂Ɋւ���l������


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


void EnemyOperation::Awake() {
	// �ϐ��̏�����(�f�t�H���g�l)
	//m_bullet = StatusVal(0.f, 0.f, 3.f);	// 3�b���Ƃɒe�𔭎�
	//m_targetDist = StatusVal(0.f, 1500.f, 2000.f);
	//m_movement = float3(0.5f, 0.5f, 0.5f);
}


void EnemyOperation::Start() {
	//m_stateMachine = m_gameObject->GetComponent<EnemyState>();
}


void EnemyOperation::Update() {
	// �^�[�Q�b�g����
	// �v���C���[�Ȃ̂��v���C���[�̖h�q�I�u�W�F�N�g�Ȃ̂��̔��f�͂����ł���

	//if (GameObject::Find("Player") != nullptr) {	// ���b�N�I��
	//	m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_ON,  true);
	//	m_stateMachine->SetTarget(GameObject::Find("Player"));
	//}
	//else {		// ���b�N�I�t
	//	m_stateMachine->SetStateActive(ENEMY_STATE::TARGET_OFF, true);
	//}
	//
	//// �^�[�Q�b�g���Ȃ�
	//if (m_stateMachine->GetTarget() == nullptr) {
	//	return;
	//}
	//
	//// �ړ�����
	//// �������Ƃ��ĉ������U�����邩�ߐڍU�������邩�͂����Ŕ��f����
	//float3 movement = float3();
	//m_targetDist.data = float3::Length(m_stateMachine->GetTarget()->m_transform->m_position, m_transform->m_position);
	//if (m_targetDist.data < m_targetDist.min) {
	//	movement.z += -m_movement.z;
	//}
	//else if (m_targetDist.data >= m_targetDist.max) {
	//	movement.z += m_movement.z;
	//}
	//else {
	//	movement.x += m_movement.x;
	//	m_bullet.data += Frame::GetInstance().GetDeltaTime();
	//}
	//if (movement.x == 0.f && movement.y == 0.f && movement.z == 0.f) {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::IDOL, true);
	//}
	//else {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::MOVE, true);
	//	m_stateMachine->SetMovement(movement);
	//}
	//
	//// �ˌ��U��
	//if (m_bullet.data >= m_bullet.max) {
	//	m_stateMachine->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	//	m_bullet.data = m_bullet.min;
	//}
}


// EOF