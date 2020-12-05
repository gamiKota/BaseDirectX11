/**
 * @file GameObject3D.cpp
 */


/**
 * @include
 */
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "System.h"


GameObject3D::GameObject3D() : m_model(E_MODEL_NONE), GameObject("GameObject3D") {
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
}


GameObject3D::GameObject3D(E_MODEL m_model, std::string name, std::string tag) : 
	m_model(m_model), GameObject(name, tag) {
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
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
	ModelManager::GetInstance().Draw(m_model, m_transform->GetMatrix());
	if (GetComponent<Collision>() != nullptr) {
		GetComponent<Collision>()->DebugDraw();
	}
	GameObject::Draw();
}


// EOF