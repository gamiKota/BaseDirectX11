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
	m_transform->m_scale = { 100.f, 100.f, 0 };
}


GameObjectUI::GameObjectUI(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	m_layer(layer), m_texture(texture), GameObject(name, tag) {
	m_transform->m_scale = { 100.f, 100.f, 0 };
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
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	DrawPolygon(D3DClass::GetInstance().GetDeviceContext());
	
	GameObject::Draw();
}


// EOF