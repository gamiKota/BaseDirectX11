/**
 * @file Object.cpp
 */


/*
 * @include
 */
#include "Object.h"
#include <list>
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "System.h"


void Object::Instantiate(Object* obj, float3 transform, Quaternion rotate, float3 scale) {
	if (obj == nullptr)	return;

	GameObject* gameObject = dynamic_cast<GameObject*>(obj);
	if (gameObject != nullptr) {
		gameObject->m_transform->m_position = transform;
		gameObject->m_transform->m_rotate = rotate;
		gameObject->m_transform->m_scale = scale;
	}

	SceneManager::GetInstance().m_scene->m_listObject.push_back(obj);
}


void Object::Destroy(Object* obj) {
	if (obj == nullptr)	return;
	// こっちのが安全だわイテレータうんち
	std::list<Object*> *objectList = &SceneManager::GetInstance().m_scene->m_listObject;
	for (auto temp : SceneManager::GetInstance().m_scene->m_listObject) {
		if (temp == obj) {
			temp->m_isActive = false;
		}
	}
}


// 一旦なしって言ったじゃん!
void Object::DontDestroyOnLoad(Object* obj) {
	//SceneManager::GetInstance().m_DontDestroyObject.push_back(obj);
}


// EOF