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



bool GameObject3DKdSort(GameObject* gameObject1, GameObject* gameObject2) {
	GameObject3D* temp1 = dynamic_cast<GameObject3D*>(gameObject1);
	GameObject3D* temp2 = dynamic_cast<GameObject3D*>(gameObject2);
	if (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->m_material.Kd.w > temp2->m_material.Kd.w) {
			return true;
		}
	}
	return false;
}



Scene::Scene() : m_isUpdate(false) {
}


Scene::~Scene() {
	for (auto obj : m_listObject) {
		delete obj;
	}
	m_listObject.clear();
}


void Scene::Init() {
	for (auto obj : m_listObject) {	// �o�b�t�@�ł��ƃC�e���[�^�̊֌W��Start�֐������s����Ȃ��I�u�W�F�N�g���o�Ă���
		obj->Init();
	}

	// �������̏I��
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

	// Collision�R���|�[�l���g�����I�u�W�F�N�g�̑������蔻��
	std::list<GameObject*> *collisionList = &m_listObject;
	for (auto i = collisionList->begin(); i != collisionList->end(); i++) {
		for (auto j = i; j != collisionList->end(); j++) {
			if (j == i)	continue;	// ���񂿏���
			GameObject *temp1 = *i;
			GameObject *temp2 = *j;
			if (temp1->GetComponent<Collision>() != nullptr && temp2->GetComponent<Collision>() != nullptr) {
				if (Collision::OBB(*temp1->GetComponent<Collision>(), *temp2->GetComponent<Collision>())) {
					// �f�o�b�O�pHit
					//temp1->GetComponent<Collision>()->SetHit();
					//temp2->GetComponent<Collision>()->SetHit();
					// �����������ɌĂ΂��֐�
					temp1->OnCollision(temp2);
					temp2->OnCollision(temp1);
					// �����G���W���@�\�H
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

	// �\�[�g
	m_listObject.sort(GameObject3DKdSort);
	m_listObject.sort(GameObjectUISort);
}

void Scene::Draw() {

	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetZBuffer(false);			// Z�o�b�t�@����
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
	D3DClass::GetInstance().SetZWrite(true);

	// 3D���f��
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObject3D*>(obj) != nullptr)
			obj->Draw();
	}

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);
	D3DClass::GetInstance().SetZBuffer(false);
	// �r���{�[�h
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObjectMesh*>(obj) != nullptr)
			obj->Draw();
	}


	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetZBuffer(false);			// Z�o�b�t�@����
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