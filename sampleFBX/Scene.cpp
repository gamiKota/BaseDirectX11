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
#include "Rigidbody.h"

#include "input.h"
#include "MeshBullet.h"
#include "tree.h"

#include "System.h"



MeshBullet g_mesh;


Scene::Scene() : m_isUpdate(false) {
}


Scene::~Scene() {
	for (auto obj : m_listObject) {
		delete obj;
	}
	m_listObject.clear();
}


void Scene::Init() {

	g_mesh.Awake();

	// �؂̏�����
	InitTree();
	for (int nCntCoin = 0; nCntCoin < 20; ++nCntCoin) {
		float fPosX, fPosY, fPosZ;

		fPosX = (float)(rand() % 12000) / 10.0f - 600.0f;
		fPosY = 0.0f;
		fPosZ = (float)(rand() % 12000) / 10.0f - 600.0f;
		SetTree(XMFLOAT3(fPosX, fPosY, fPosZ), 60.0f, 190.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	auto buff = m_listObject;
	for (auto obj : buff) {
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

	UninitTree();
	g_mesh.Uninit();
}

void Scene::Update() {

	if (Input::isTrigger('K')) {
		g_mesh.Fire(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 1.f));
	}


	g_mesh.Update();
	UpdateTree();

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
					temp1->GetComponent<Collision>()->SetHit();
					temp2->GetComponent<Collision>()->SetHit();
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
	// �r���{�[�h

	g_mesh.Draw();
	DrawTree();


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