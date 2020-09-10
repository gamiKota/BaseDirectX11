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
#include "D3DClass.h"
#include "Collision.h"
#include "System.h"


Scene::Scene() : m_isUpdate(true) {
}


Scene::~Scene() {
	for (auto obj : m_listObject) {
		delete obj;
	}
	m_listObject.clear();
}


void Scene::Init() {
	m_isUpdate = true;

	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Init();
	}

	// �������̏I��
	m_isUpdate = false;
}

void Scene::Uninit() {
	auto buff = m_listObject;
	for (auto obj : buff) {
		obj->Uninit();
	}
}

void Scene::Update() {
	auto buff = m_listObject;
	for (auto obj : buff)
		obj->Update();

	buff = m_listObject;
	for (auto obj : buff)
		obj->LastUpdate();

	buff = m_listObject;
	for (auto obj : buff) {
		if (obj->GetActive())	continue;
		m_listObject.remove(obj);
		delete obj;
	}

	// Collision�R���|�[�l���g�����I�u�W�F�N�g�̑������蔻��
	std::list<GameObject*> *collisionList = &m_listObject;
	for (auto i = collisionList->begin(); i != collisionList->end(); i++) {
		for (auto j = i; j != collisionList->end(); j++) {
			if (j == i)	continue;	// ���񂿏���
			GameObject *temp1 = *i;
			GameObject *temp2 = *j;
			if (temp1->GetComponent<Collision>() != nullptr && temp2->GetComponent<Collision>() != nullptr) {
				if (Collision::AABB(*temp1->GetComponent<Collision>(), *temp2->GetComponent<Collision>())) {
					temp1->OnCollision(temp2);
					temp2->OnCollision(temp1);
				}
			}
		}
	}
}

void Scene::Draw() {

	// �w�i�Ȃǂ�UI
	auto buff = m_listObject;
	for (auto obj : buff) {
		GameObjectUI* background = dynamic_cast<GameObjectUI*>(obj);
		if (background != nullptr && background->m_layer < E_LAYER::UI) {
			obj->Draw();
		}
	}

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);

	// 3D���f��
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObject3D*>(obj) != nullptr)
			obj->Draw();
	}

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Z�o�b�t�@����
	D3DClass::GetInstance().SetZBuffer(false);

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