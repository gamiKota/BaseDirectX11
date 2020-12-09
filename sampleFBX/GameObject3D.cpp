/**
 * @file GameObject3D.cpp
 */


/**
 * @include
 */
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "D3DClass.h"
#include "System.h"


GameObject3D::GameObject3D() : m_model(E_MODEL_NONE), GameObject("GameObject3D") {
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
	m_shader = E_SHADER_FBX;
}


GameObject3D::GameObject3D(E_MODEL m_model, std::string name, std::string tag) : 
	m_model(m_model), GameObject(name, tag) {
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
	m_shader = E_SHADER_FBX;
}


GameObject3D::~GameObject3D() {
}


void GameObject3D::Init() {
	GameObject::Init();
}


void GameObject3D::Uninit() {
	GameObject::Uninit();
}


void GameObject3D::Update() {
	GameObject::Update();
}


void GameObject3D::LastUpdate() {
	GameObject::LastUpdate();

	ModelManager::GetInstance().Update(m_model);
}


void GameObject3D::Draw() {
	// 前面カリング (FBXは表裏が反転するため)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);
	D3DClass::GetInstance().SetZWrite(true);
	ModelManager::GetInstance().Draw(this);
	if (GetComponent<Collision>() != nullptr &&
		GetTag() != "Land") {
		GetComponent<Collision>()->DebugDraw();
	}
	GameObject::Draw();
}


// EOF