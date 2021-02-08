#include "PlayerBullet.h"
#include "Bullet.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "Text.h"
#include "System.h"

void PlayerBullet::Start() {
	m_ui = new GameObjectUI(E_LAYER::UI, E_TEXTURE_NUMBER, "PlayerBullet", "PlayerBullet");
	GameObject::Instantiate(m_ui, float3(450.f, -250.f, 0.f), Quaternion::identity, float3(200.f, 60.f, 0.f));
}

void PlayerBullet::Update() {
	
}

void PlayerBullet::shot(E_BULLET eBullet) {
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, m_transform->m_position + m_transform->m_forward * 200.f, m_transform->m_rotation);
	obj->AddComponent<Bullet>();
}

// EOF