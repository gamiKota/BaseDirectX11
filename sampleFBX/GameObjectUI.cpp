/**
 * @file GameObjectUI.cpp
 */


/**
 * @include
 */
#include "GameObjectUI.h"
#include "D3DClass.h"
#include "polygon.h"
#include "System.h"


GameObjectUI::GameObjectUI(E_LAYER layer) : m_layer(layer), m_texture(E_TEXTURE_NONE), GameObject("GameObjectUI") {

}


GameObjectUI::GameObjectUI(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	m_layer(layer), m_texture(texture), GameObject(name, tag) {
	// 変数の初期化
	m_transform->m_scale = { 100.f, 100.f, 0 };
	m_color = float3(1.f, 1.f, 1.f);
	m_alpha = 1.f;
	m_vs = VS_2D;
	m_ps = PS_2D;
}


GameObjectUI::~GameObjectUI() {
}


void GameObjectUI::Init() {

	GameObject::Init();
}


void GameObjectUI::Uninit() {
	GameObject::Uninit();
}


void GameObjectUI::Update() {
	GameObject::Update();
}


void GameObjectUI::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectUI::Draw() {

	ShaderManager* shader = &ShaderManager::GetInstance();

	shader->BindVS(m_vs);
	shader->BindPS(m_ps);
	D3DClass::GetInstance().GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	D3DClass::GetInstance().GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	D3DClass::GetInstance().GetDeviceContext()->GSSetShader(NULL, NULL, 0);
	D3DClass::GetInstance().GetDeviceContext()->CSSetShader(NULL, NULL, 0);

	SetPolygonTexture(TextureManager::GetInstance().Get(m_texture));
	SetPolygonPos(m_transform->m_position.x, m_transform->m_position.y);
	SetPolygonSize(m_transform->m_scale.x, m_transform->m_scale.y);
	SetPolygonAngle(Quaternion::EulerAngle(m_transform->m_rotation).z);
	SetPolygonUV(0.f, 0.f);
	SetPolygonFrameSize(1.f, 1.f);
	SetPolygonColor(m_color.x, m_color.y, m_color.z);
	SetPolygonAlpha(m_alpha);
	DrawPolygon(D3DClass::GetInstance().GetDeviceContext());
	
	GameObject::Draw();
}


bool GameObjectUISort(GameObject* obj1, GameObject* obj2) {
	GameObjectUI* temp1 = dynamic_cast<GameObjectUI*>(obj1);
	GameObjectUI* temp2 = dynamic_cast<GameObjectUI*>(obj2);
	if (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->m_layer < temp2->m_layer) {
			return true;
		}
	}
	return false;
}


// EOF