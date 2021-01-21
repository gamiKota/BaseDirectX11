/*
 */


#include "MissionState.h"
#include "System.h"


void MissionState::Initialize() {
	// ��Ԃ̒ǉ�
	StateMachine::AddState(new MissionState::StartState(this), true);
	StateMachine::AddState(new MissionState::UpdateState(this));
	StateMachine::AddState(new MissionState::EndSuccess(this));
	StateMachine::AddState(new MissionState::EndFailed(this));
	StateMachine::AddState(new MissionState::CreateEnemy(this));
}


void MissionState::Update() {
	// ��ԂɈˑ����Ȃ����ʏ���
}


/**************************************************************************************************
 * @state �~�b�V�����J�n
 *************************************************************************************************/
void MissionState::StartState::Start() {
	// �����I�������̏�Ԃ��L����������A�N�e�B�u�l������
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::StartState::Update() {
	// �Ȃ񂩊J�n���S�Ƃ��o��
	m_parent->SetStateActive(MISSION_STATE::UPDATE, true);
}

void MissionState::StartState::OnDestoy() {

}


/**************************************************************************************************
 * @state �~�b�V�����X�V
 *************************************************************************************************/
void MissionState::UpdateState::Start() {
}

void MissionState::UpdateState::Update() {
}

void MissionState::UpdateState::OnDestoy() {
}


/**************************************************************************************************
 * @state �~�b�V�����I��(����)
 *************************************************************************************************/
void MissionState::EndSuccess::Start() {
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::EndSuccess::Update() {
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::EndSuccess::OnDestoy() {
}


/**************************************************************************************************
 * @state �~�b�V�����I��(���s)
 *************************************************************************************************/
void MissionState::EndFailed::Start() {
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
}

void MissionState::EndFailed::Update() {
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
}

void MissionState::EndFailed::OnDestoy() {
}


/**************************************************************************************************
 * @state �ʏ�G����
 *************************************************************************************************/
void MissionState::CreateEnemy::Start() {
}

void MissionState::CreateEnemy::Update() {
}

void MissionState::CreateEnemy::OnDestoy() {
}

// EOF