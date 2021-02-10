#include "PlayerUI.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "Material.h"
#include "Frame.h"
#include "System.h"

void PlayerUI::Start() {
	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	obj->m_vs = E_VS::VS_2D_FRAME;
	obj->m_ps = E_PS::PS_2D_FRAME;
	m_material = obj->m_material;
	m_material->m_diffuse = PLAYER_UI_DIFFUSE;
	m_material->m_inDiffuse = PLAYER_UI_IN_DIFFUSE;
	InitError();
}

void PlayerUI::Update() {
	if (m_isError) {
		m_errorTime -= Frame::GetInstance().GetDeltaTime();
		if (m_errorTime < 0.f) {
			if (m_errorFunc != nullptr) {
				m_errorFunc();
				m_errorFunc = nullptr;
				InitError();
			}
		}
		m_swithTime += Frame::GetInstance().GetDeltaTime();
		if (m_swithTime > 0.1f) {
			m_isSwith ^= true;
			m_swithTime = 0.f;
		}
		m_material->m_inDiffuse = (m_isSwith) ? PLAYER_UI_ERROR : PLAYER_UI_IN_DIFFUSE;
	}
}

void PlayerUI::InitError() {
	m_errorTime = 0.f;
	m_isError = false;
	m_swithTime = 0.f;
	m_isSwith = false;
	m_material->m_inDiffuse = PLAYER_UI_IN_DIFFUSE;
}

PlayerUI* PlayerUI::SetError(float errorTime) {
	m_isError = true;
	m_isError = true;
	m_errorTime = errorTime;
	return this;
}

// EOF