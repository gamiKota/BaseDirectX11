/**
 * @file GameObject3D.cpp
 */


/**
 * @include
 */
#include "GameObject3D.h"
#include "Collision.h"


GameObject3D::GameObject3D(E_MODEL m_model) : m_model(m_model), GameObject("GameObject3D") {
	
}


GameObject3D::GameObject3D(E_MODEL m_model, std::string name, std::string tag) : 
	m_model(m_model), GameObject(name, tag) {
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
	//GameObject::Draw();
}