#include "TitleManager.h"
#include "input.h"
#include "Frame.h"
#include "GameObjectUI.h"
#include "SceneManager.h"
#include "System.h"

void TitleManager::Start() {
	m_time = 0.f;
	m_isLogo = false;
}

void TitleManager::Update() {
	m_time += Frame::GetInstance().GetDeltaTime();
	if (Input::isTrigger(VK_RETURN)) {
		if (!m_isLogo) {
			// �r�ō�탍�S
			this->CreateLogo();
		}
		else {
			SceneManager::GetInstance().LoadScene(E_SCENE::GAME);
		}
	}
	if (m_time >= 3.f && !m_isLogo) {
		this->CreateLogo();
	}
}


void TitleManager::CreateLogo() {
	// �r�ō�탍�S
	GameObjectUI* ui = new GameObjectUI(E_LAYER::UI, E_TEXTURE_SELECT01, "title", "title");
	GameObject::Instantiate(ui, float3(), Quaternion::identity, float3(200.f, 70.f, 0.f));
	m_isLogo = true;
}