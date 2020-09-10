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
#include "System.h"


using namespace DirectX;


std::list<GameObject*>	GameObject::m_listFinds = std::list<GameObject*>();		//!< 複数リスト取得用


GameObject::GameObject(std::string name, std::string tag) : Object(name), m_tag(tag) {
	m_transform = AddComponent<Transform>();
}


GameObject::~GameObject() {
	for (auto com : m_listComponent)
		delete com;
	m_listComponent.clear();
}


void GameObject::Init() {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->Start();
}


void GameObject::Uninit() {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->Uninit();
}


void GameObject::Update() {
	auto buff = m_listComponent;
	for (auto com : buff)
		com->Update();
}


void GameObject::LastUpdate() {
	PrintDebugProc("name = %s, Instance = %d, pos = %.2f, %.2f, %.2f\n", ToString(), GetInstanceID(),
		m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z);

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

// わかんにゃーい！！！！！！！！！
std::list<GameObject*> GameObject::FindGameObjectsWithTag(std::string tag) {
	m_listFinds.clear();
	for (auto obj : SceneManager::GetInstance().m_scene->m_listObject) {
		if (obj->m_tag == tag) {
			m_listFinds.push_back(obj);
		}
	}
	return m_listFinds;
}

void GameObject::Instance(GameObject* obj, XMFLOAT3 transform) {
	if (obj == nullptr)	return;

	obj->m_transform->m_position = transform;

	SceneManager::GetInstance().m_scene->m_listObject.push_back(obj);
}


// EOF