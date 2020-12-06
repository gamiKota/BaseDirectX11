/**
 * @file Scene.cpp
 */


/**
 * @include
 */
#include "Scene.h"
#include "GameObject.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectMesh.h"
#include "D3DClass.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "System.h"



Scene::Scene() : m_isUpdate(false) {
}


Scene::~Scene() {
	for (auto obj : m_listObject) {
		delete obj;
	}
	m_listObject.clear();
}


void Scene::Init() {
	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Init();
	}

	// 初期化の終了
	m_isUpdate = true;
}

void Scene::Uninit() {
	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Uninit();
	}

	m_isUpdate = false;

	TextureManager::GetInstance().Release();
}

void Scene::Update() {
	auto buff = m_listObject;
	for (auto obj : buff)
		obj->Update();

	buff = m_listObject;
	for (auto obj : buff)
		obj->LastUpdate();

	// Collisionコンポーネントを持つオブジェクトの総当たり判定
	std::list<GameObject*> *collisionList = &m_listObject;
	for (auto i = collisionList->begin(); i != collisionList->end(); i++) {
		for (auto j = i; j != collisionList->end(); j++) {
			if (j == i)	continue;	// うんち処理
			GameObject *temp1 = *i;
			GameObject *temp2 = *j;
			if (temp1->GetComponent<Collision>() != nullptr && temp2->GetComponent<Collision>() != nullptr) {
				if (Collision::OBB(*temp1->GetComponent<Collision>(), *temp2->GetComponent<Collision>())) {
					// デバッグ用Hit
					//temp1->GetComponent<Collision>()->SetHit();
					//temp2->GetComponent<Collision>()->SetHit();
					// 当たった時に呼ばれる関数
					temp1->OnCollision(temp2);
					temp2->OnCollision(temp1);
					// 物理エンジン機能？
					if (temp1->GetComponent<Rigidbody>() != nullptr && temp2->GetComponent<Rigidbody>() != nullptr) {
						Rigidbody::ShiftCollision(temp1, temp2);
					}
				}
			}
		}
	}

	buff = m_listObject;
	for (auto obj : buff) {
		if (obj->GetActive())	continue;
		obj->Uninit();
		m_listObject.remove(obj);
		SAFE_DELETE(obj);
	}
}

void Scene::Draw() {

	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetZBuffer(false);			// Zバッファ無効
	// 背景などのUI
	auto buff = m_listObject;
	for (auto obj : buff) {
		GameObjectUI* background = dynamic_cast<GameObjectUI*>(obj);
		if (background != nullptr && background->m_layer < E_LAYER::UI) {
			obj->Draw();
		}
	}

	// 前面カリング (FBXは表裏が反転するため)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);

	// 3Dモデル
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObject3D*>(obj) != nullptr)
			obj->Draw();
	}

	// ビルボード
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObjectMesh*>(obj) != nullptr)
			obj->Draw();
	}


	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetZBuffer(false);			// Zバッファ無効
	// 2DUI
	buff = m_listObject;
	for (auto obj : buff) {
		GameObjectUI* UI = dynamic_cast<GameObjectUI*>(obj);
		if (UI != nullptr && UI->m_layer >= E_LAYER::UI) {
			obj->Draw();
		}
	}
}


void Scene::Shutdown() {
	for (auto obj : m_listObject)
		delete obj;

	m_listObject.clear();
}


// EOF