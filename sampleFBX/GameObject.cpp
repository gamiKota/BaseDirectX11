/**
 * @file GameObject
 */


/**
 * include
 */
#include "GameObject.h"
#include "Component.h"
#include "D3DClass.h"
#include "SceneManager.h"
#include "Scene.h"
#include "debugproc.h"
#include "imgui.h"
#include "System.h"


using namespace DirectX;


std::list<GameObject*>	GameObject::m_listFinds = std::list<GameObject*>();		//!< 複数リスト取得用


GameObject::GameObject(std::string name, std::string tag) : Object(name), m_tag(tag) {
	m_transform = AddComponent<Transform>();
	m_transform->m_scale = float3(1.f, 1.f, 1.f);
}


GameObject::~GameObject() {
	for (auto com : m_listComponent)
		delete com;
	m_listComponent.clear();
}


void GameObject::Init() {
	//auto buff = m_listComponent;
	for (auto com : m_listComponent)
		com->Start();
}


void GameObject::Uninit() {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->Uninit();
}


void GameObject::Update() {
	auto buff = m_listComponent;
	for (auto com : buff) {
		com->Update();
	}
#if _DEBUG
	//if (ImGui::CollapsingHeader(m_name.c_str())) {	// IDが連動するっぽい？
	if (ImGui::TreeNode(m_name.c_str())) {
		for (auto com : buff) {
			if (ImGui::TreeNode(com->m_name.c_str())) {
				com->SetImGuiVal();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
#endif
}


void GameObject::LastUpdate() {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->LastUpdate();
}


void GameObject::Draw() {
	
}


void GameObject::OnCollision(GameObject* obj) {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->OnCollision(obj);
}


GameObject* GameObject::Find(std::string name) {
	for (auto objectList : SceneManager::GetInstance().m_scene->m_listObject) {
		if (objectList->m_name == name) {
			return objectList;
		}
	}
	return nullptr;
}


GameObject* GameObject::FindGameObjectWithTag(std::string tag) {
	for (auto objectList : SceneManager::GetInstance().m_scene->m_listObject) {
		if (objectList->m_tag == tag) {
			return objectList;
		}
	}
	return nullptr;
}


std::list<GameObject*> GameObject::FindGameObjectsWithTag(std::string tag) {
	m_listFinds.clear();
	for (auto obj : SceneManager::GetInstance().m_scene->m_listObject) {
		if (obj->m_tag == tag) {
			m_listFinds.push_back(obj);
		}
	}
	return m_listFinds;
}


// EOF