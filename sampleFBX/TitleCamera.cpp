
#include "TitleCamera.h"
#include "Transform.h"
#include "GameObject.h"


void TitleCamera::Start() {
	CCamera::Start();
	m_fFarZ = 25000.0f;
	//m_player = GameObject::FindGameObjectWithTag("Player");
}

void TitleCamera::Update() {
	GameObject* player = GameObject::FindGameObjectWithTag("Player");
	if (player != nullptr)
		m_transform->LookAt(player->m_transform->m_position);
}