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

	if (Input::isTrigger(VK_RETURN)) {
		if (!m_isLogo) {
			// ürñ≈çÏêÌÉçÉS
			GameObjectUI* ui = new GameObjectUI(E_LAYER::UI, E_TEXTURE_SELECT01, "title", "title");
			GameObject::Instantiate(ui, float3(), Quaternion::identity, float3(200.f, 70.f, 0.f));
			m_isLogo = true;
		}
		else {
			SceneManager::GetInstance().LoadScene(E_SCENE::GAME);
		}
	}
}