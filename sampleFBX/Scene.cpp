/**
 * @file Scene.cpp
 */


/**
 * @include
 */
#include "Scene.h"
#include "GameObject.h"
#include "D3DClass.h"
#include "Collision.h"
#include "System.h"


Scene::Scene() : m_isUpdate(true) {
}


Scene::~Scene() {
	for (auto obj : m_listObject) {
		delete obj;
	}
	for (auto obj : m_listUI) {
		delete obj;
	}
	m_listObject.clear();
	m_listUI.clear();
}


void Scene::Init() {
	m_isUpdate = true;

	for (auto obj : SceneManager::GetInstance().m_DontDestroyObject) {
		m_listUI.push_back(obj);
	}

	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Start();	// Start? Init? どっち？
	}
	buff = m_listUI;
	for (auto obj : buff) {
		obj->Start();
	}

	// 初期化の終了
	m_isUpdate = false;
}

void Scene::Uninit() {
	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Uninit();
	}
	buff = m_listUI;
	for (auto obj : buff) {
		obj->Uninit();
	}
}

void Scene::Update() {
	auto buff = m_listObject;
	for (auto obj : buff)
		obj->Update();
	buff = m_listUI;
	for (auto obj : buff)
		obj->Update();

	buff = m_listObject;
	for (auto obj : buff) {
		if (obj->GetActive())	continue;
		m_listObject.remove(obj);
		delete obj;
	}
	buff = m_listUI;
	for (auto obj : buff) {
		if (obj->GetActive())	continue;
		m_listUI.remove(obj);
		delete obj;
	}

	// Collisionコンポーネントを持つオブジェクトの総当たり判定
	//std::list<GameObject*> *collisionList = &m_listObject;
	//for (auto i = collisionList->begin(); i != collisionList->end(); i++) {
	//	for (auto j = i; j != collisionList->end(); j++) {
	//		if (j == i)	continue;	// うんち処理
	//		GameObject *temp1 = *i;
	//		GameObject *temp2 = *j;
	//		if (temp1->GetComponent<Collision>() != nullptr && temp2->GetComponent<Collision>() != nullptr) {
	//			if (Collision::AABB(*temp1->GetComponent<Collision>(), *temp2->GetComponent<Collision>())) {
	//			}
	//		}
	//	}
	//}
}

void Scene::Draw() {
	// 背景などのUI

	// 前面カリング (FBXは表裏が反転するため)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);

	// 3Dモデル
	auto buff = m_listObject;
	for (auto obj : buff)
		obj->Draw();

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(false);

	// 2DUI
	buff = m_listUI;
	for (auto obj : buff)
		obj->Draw();
}


void Scene::Shutdown() {
	for (auto obj : m_listObject)
		delete obj;
	for (auto obj : m_listUI)
		delete obj;

	m_listObject.clear();
	m_listUI.clear();
}


// EOF