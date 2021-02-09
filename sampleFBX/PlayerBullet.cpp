#include "PlayerBullet.h"
#include "Bullet.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectText.h"
#include "Text.h"
#include "Material.h"
#include "Frame.h"
#include "System.h"


constexpr int MAX_BULLET = 10;


void PlayerBullet::Start() {
	// 弾数表示
	m_text = new GameObjectText((E_LAYER)((int)E_LAYER::UI + 3), E_TEXTURE_TEXT, "PlayerBulletNum", "PlayerBulletNum");
	GameObject::Instantiate(m_text, float3(580.f, -275.f, 0.f), Quaternion::identity, float3());
	//m_text->m_vs = E_VS::VS_2D_FRAME;
	m_text->m_ps = E_PS::PS_COLOR;	// テキストはポリゴンの表示の仕方が特殊だから無理っぽい
	m_text->m_material->m_diffuse = DirectX::XMFLOAT4(0.f, 0.9f, 1.f, 1.f);
	m_text->SetFontSize(24.f);
	// 弾UI
	m_weapon = new GameObjectUI((E_LAYER)((int)E_LAYER::UI + 3), E_TEXTURE_WEAPONS, "PlayerWeapon", "PlayerWeapon");
	GameObject::Instantiate(m_weapon, float3(475.f, -275.f, 0.f), Quaternion::identity, float3(150.f, 40.f, 0.f));
	m_weapon->m_vs = E_VS::VS_2D_FRAME;
	m_weapon->m_ps = E_PS::PS_2D_FRAME;
	m_weapon->m_material->m_diffuse = DirectX::XMFLOAT4(0.f, 0.9f, 1.f, 1.f);

	// 変数の初期化
	m_numBullet = MAX_BULLET;
	m_isReload = false;
	m_isReloadTime = 0.f;

	m_errorTime = 0.f;
	m_isError = false;

	// 取り合えずここ
	ShaderManager::GetInstance().SetTexturePS(TextureManager::GetInstance().Get(E_TEXTURE_FRAME), 3);
}

void PlayerBullet::Update() {
	m_text->SetText("%d", m_numBullet);
	DirectX::XMFLOAT4 d = DirectX::XMFLOAT4(0.f, 0.9f, 1.f, 0.2f);
	if (m_isReload) {
		m_isReloadTime += Frame::GetInstance().GetDeltaTime();
		if (m_isReloadTime > 2.f) {
			m_isReloadTime = 0.f;
			m_numBullet = MAX_BULLET;
			m_isReload = false;

			m_errorTime = 0.f;
			m_isError = false;
		}


		else {
			m_errorTime += Frame::GetInstance().GetDeltaTime();
			if (m_errorTime > 0.1f) {
				m_isError ^= true;
				m_errorTime = 0.f;
			}
			d = (m_isError) ? d : DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		}
	}
	m_weapon->m_material->m_inDiffuse = d;
}

void PlayerBullet::shot(E_BULLET eBullet) {
	if (m_isReload)	return;
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, m_transform->m_position + m_transform->m_forward * 200.f, m_transform->m_rotation);
	obj->AddComponent<Bullet>();
	m_numBullet--;
	m_isReload = (m_numBullet <= 0) ? true : false;
}

// EOF