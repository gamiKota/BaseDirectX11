#include "PlayerBullet.h"
#include "Bullet.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectText.h"
#include "Material.h"
#include "Frame.h"
#include "PlayerUI.h"
#include "System.h"


constexpr int MAX_BULLET = 10;
constexpr float TIME_RELOAD = 2.f;


void PlayerBullet::Start() {
	// 弾数表示
	m_text = new GameObjectText((E_LAYER)((int)E_LAYER::UI + 3), E_TEXTURE_TEXT, "PlayerBulletNum", "PlayerBulletNum");
	GameObject::Instantiate(m_text, float3(580.f, -275.f, 0.f), Quaternion::identity, float3());
	//m_text->m_vs = E_VS::VS_2D_FRAME;
	m_text->m_ps = E_PS::PS_COLOR;	// テキストはポリゴンの表示の仕方が特殊だから無理っぽい
	m_text->m_material->m_diffuse = PLAYER_UI_DIFFUSE;
	m_text->SetFontSize(24.f);
	// 弾UI
	m_weapon = new GameObjectUI((E_LAYER)((int)E_LAYER::UI + 3), E_TEXTURE_WEAPONS, "PlayerWeapon", "PlayerWeapon");
	GameObject::Instantiate(m_weapon, float3(475.f, -275.f, 0.f), Quaternion::identity, float3(150.f, 40.f, 0.f));
	m_weapon->AddComponent<PlayerUI>();

	// 変数の初期化
	m_numBullet = MAX_BULLET;

	// 取り合えずここ
	ShaderManager::GetInstance().SetTexturePS(TextureManager::GetInstance().Get(E_TEXTURE_FRAME), 3);
}

void PlayerBullet::Update() {
	m_text->SetText("%d", m_numBullet);
}

void PlayerBullet::shot(E_BULLET eBullet) {
	if (m_numBullet <= 0)	return;
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, m_transform->m_position + m_transform->m_forward * 200.f, m_transform->m_rotation);
	obj->AddComponent<Bullet>();
	m_numBullet--;
	if (m_numBullet <= 0) {
		m_weapon->GetComponent<PlayerUI>()->SetError(TIME_RELOAD)->OnComplete(
			[bullet = this]() {
			bullet->m_numBullet = MAX_BULLET;
		});
	}
}

// EOF