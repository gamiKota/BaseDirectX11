#include "PlayerBullet.h"
#include "Bullet.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectText.h"
#include "Text.h"
#include "System.h"

void PlayerBullet::Start() {
	m_text = new GameObjectText((E_LAYER)((int)E_LAYER::UI + 3), E_TEXTURE_TEXT, "PlayerBulletNum", "PlayerBulletNum");
	GameObject::Instantiate(m_text, float3(475.f, -225.f, 0.f), Quaternion::identity, float3());
	m_numBullet = 80;
	m_text->SetFontSize(24.f);
}

void PlayerBullet::Update() {
	m_text->SetText("%d", m_numBullet);
}

void PlayerBullet::shot(E_BULLET eBullet) {
	m_numBullet--;
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, m_transform->m_position + m_transform->m_forward * 200.f, m_transform->m_rotation);
	obj->AddComponent<Bullet>();
}

// EOF