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

}


GameObjectUI::~GameObjectUI() {
}


void GameObjectUI::Init() {
	// 変数の初期化
	m_transform->m_scale = { 100.f, 100.f, 0 };
	m_color = float3(1.f, 1.f, 1.f);
	m_shader = E_SHADER_2D::_NORMAL;

	GameObject::Init();
}


void GameObjectUI::Uninit() {
	GameObject::Uninit();
}


void GameObjectUI::Update() {
	GameObject::Update();
	if (m_layer == E_LAYER::BILLBOARD) {

	}
}


void GameObjectUI::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectUI::Draw() {
	SetPolygonTexture(TextureManager::GetInstance().Get(m_texture));
	SetPolygonPos(m_transform->m_position.x, m_transform->m_position.y);
	SetPolygonSize(m_transform->m_scale.x, m_transform->m_scale.y);
	SetPolygonAngle(m_transform->m_rotate.z);
	SetPolygonUV(0.f, 0.f);
	SetPolygonFrameSize(1.f, 1.f);
	SetPolygonColor(m_color.x, m_color.y, m_color.z);
	SetPolygonAlpha(1.0f);
	SetPolygonShader(m_shader);
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