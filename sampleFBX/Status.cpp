/**
 * @file Status.cpp
 */


/**
 * @include
 */
#include "Status.h"
#include "imgui.h"
#include "Frame.h"
#include "System.h"



Status::Status() : Component(),
	m_HP(0.f), m_AttakPower(0.f),
	m_hitStopTime(StatusVal()), m_deleteTime(StatusVal()), m_bulletTime(StatusVal()),
	m_isDead(0) {
}


void Status::Start() {

}


void Status::Update() {
	if (m_HP <= 0.f || m_isDead) {
		// HP��0�ɂȂ������_�ŃX�e�[�^�X�֘A�̑S�Ă̏��̍X�V���Ւf
	}

	// �f���^�^�C��
	float deltaTime = Frame::GetInstance().GetDeltaTime();

	// ���Ŋ֘A�̍X�V
	if (m_HP <= 0.f && !m_isDead) {
		if (m_deleteTime.data < m_deleteTime.max) {	// �ݒ肵�����Ŏ��Ԃɓ��B
			m_deleteTime.data += deltaTime;
		}
		else {
			m_isDead = true;
		}
	}

	// �������U���֘A�̍X�V
	if (m_bulletTime.data < m_bulletTime.max) {
		m_bulletTime.data += deltaTime;	// ���Ԃ̍X�V
	}
}


void Status::SetImGuiVal() {
	ImGui::InputFloat("HP", &m_HP);
	ImGui::InputFloat("Atack", &m_AttakPower);
	ImGui::InputFloat("HitStop", &m_hitStopTime.data);
}


// EOF