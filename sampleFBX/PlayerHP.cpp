#include "PlayerHP.h"
#include "GameObjectUI.h"
#include "PlayerUI.h"
#include "System.h"

void PlayerHP::Start() {
	m_hp = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_FRAME, "PlayerHp", "PlayerHp");
	GameObject::Instantiate(m_hp, float3(545.f, -315.f, 0.f), Quaternion::identity, float3(150.f, 30.f, 0.f));
	m_hp->AddComponent<PlayerUI>()->m_type = false;

	m_logo = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_FRAME, "PlayerHpLogo", "PlayerHpLogo");
	GameObject::Instantiate(m_logo, float3(435.f, -315.f, 0.f), Quaternion::identity, float3(70.f, 30.f, 0.f));
	m_logo->AddComponent<PlayerUI>()->m_type;
}

void PlayerHP::Update() {

}

// EOF