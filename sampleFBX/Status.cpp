/**
 * @file Status.cpp
 */


/**
 * @include
 */
#include "Status.h"
#include "imgui.h"



Status::Status() : Component(), m_HP(0.f), m_AttakPower(0.f), m_HitStop(0.f), m_isDead(0) {

}


void Status::Start() {

}


void Status::Update() {
	if (!m_isDead && m_HP <= 0.f) {
		m_isDead = true;
	}
}


void Status::SetImGuiVal() {
	ImGui::InputFloat("HP", &m_HP);
	ImGui::InputFloat("Atack", &m_AttakPower);
	ImGui::InputFloat("HitStop", &m_HitStop);
}


// EOF