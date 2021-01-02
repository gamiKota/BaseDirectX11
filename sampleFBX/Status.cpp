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
		// HPが0になった時点でステータス関連の全ての情報の更新を遮断
	}

	// デルタタイム
	float deltaTime = Frame::GetInstance().GetDeltaTime();

	// 消滅関連の更新
	if (m_HP <= 0.f && !m_isDead) {
		if (m_deleteTime.data < m_deleteTime.max) {	// 設定した消滅時間に到達
			m_deleteTime.data += deltaTime;
		}
		else {
			m_isDead = true;
		}
	}

	// 遠距離攻撃関連の更新
	if (m_bulletTime.data < m_bulletTime.max) {
		m_bulletTime.data += deltaTime;	// 時間の更新
	}
}


void Status::SetImGuiVal() {
	ImGui::InputFloat("HP", &m_HP);
	ImGui::InputFloat("Atack", &m_AttakPower);
	ImGui::InputFloat("HitStop", &m_hitStopTime.data);
}


// EOF