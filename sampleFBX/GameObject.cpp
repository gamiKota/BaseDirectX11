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


GameObject::GameObject() : Object("GameObject"), m_tag("none") {
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
}


GameObject::GameObject(std::string name, std::string tag) : Object(name), m_tag(tag) {
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
}


GameObject::~GameObject() {
	for (auto com : m_listComponent)
		delete com;
	m_listComponent.clear();
}


void GameObject::Awake() {
	// Awakeのタイミングとか処理順がわかんにゃい...
	//auto buff = List;
	//for (auto com : buff)
	//	com->Awake();
}


void GameObject::Start() {
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

	PrintDebugProc("name = %s, Instance = %d, pos = %.2f, %.2f, %.2f\n", ToString(), GetInstanceID(),
		m_transform._41, m_transform._42, m_transform._43);

	auto buff = m_listComponent;
	for (auto com : buff)
		com->Update();
}


void GameObject::Draw() {
	auto buff = m_listComponent;
	for (auto com : m_listComponent)
		com->Draw();
}


GameObject* GameObject::Find(std::string name) {
	for (auto objectList : SceneManager::GetInstance().m_scene->m_listObject) {
		if (objectList->m_name == name) {
			return objectList;
		}
	}
	for (auto objectList : SceneManager::GetInstance().m_scene->m_listUI) {
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
	for (auto objectList : SceneManager::GetInstance().m_scene->m_listUI) {
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
	for (auto obj : SceneManager::GetInstance().m_scene->m_listUI) {
		if (obj->m_tag == tag) {
			m_listFinds.push_back(obj);
		}
	}
	return m_listFinds;
}

void GameObject::Instance(GameObject* obj, XMFLOAT3 transform) {
	if (obj == nullptr)	return;

	obj->m_transform._41 = transform.x;
	obj->m_transform._42 = transform.y;
	obj->m_transform._43 = transform.z;

	SceneManager::GetInstance().m_scene->m_listObject.push_back(obj);
}


// EOF