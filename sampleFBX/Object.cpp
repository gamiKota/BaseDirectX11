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


void Object::Instantiate(GameObject* obj, float3 transform, Quaternion rotate, float3 scale) {
	if (obj == nullptr)	return;

	obj->m_transform->m_position = transform;
	obj->m_transform->m_rotate = rotate;
	obj->m_transform->m_scale = scale;

	SceneManager::GetInstance().m_scene->m_listObject.push_back(obj);
}


void Object::Destroy(GameObject* obj) {
	if (obj == nullptr)	return;
	
	// こっちのが安全だわイテレータうんち
	std::list<GameObject*> *objectList = &SceneManager::GetInstance().m_scene->m_listObject;
	for (auto temp : SceneManager::GetInstance().m_scene->m_listObject) {
		if (temp->GetInstanceID() == obj->GetInstanceID()) {
			temp->m_isActive = false;
		}
	}

	//std::list<GameObject*> *objectList = &SceneManager::GetInstance().m_scene->m_objectList;
	//for (auto itr = objectList->begin(); itr != objectList->end();) {
	//	GameObject *temp = *itr;
	//	if (temp->GetInstanceID() == obj->GetInstanceID()) {
	//		delete temp;
	//		temp = nullptr;
	//		itr = objectList->erase(itr);
	//		continue;
	//	}
	//	itr++;
	//}
}


void Object::DontDestroyOnLoad(GameObject* obj) {
	obj->m_isDontDestroy = true;
}


// EOF