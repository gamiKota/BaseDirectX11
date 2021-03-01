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
#include "WaterSurface.h"
#include "D3DClass.h"
#include "Collision.h"
#include "CollisionMesh.h"
#include "Rigidbody.h"
#include "debugproc.h"
#include "Material.h"
#include "Light.h"
#include "System.h"



// ����
// Start�֐���Update�֐��̍ŏ��ɌĂ΂�����
// �X�e�[�g�Ǘ�



bool GameObject3DKdSort(GameObject* gameObject1, GameObject* gameObject2) {
	GameObject3D* temp1 = dynamic_cast<GameObject3D*>(gameObject1);
	GameObject3D* temp2 = dynamic_cast<GameObject3D*>(gameObject2);
	if (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->m_material->m_diffuse.w > temp2->m_material->m_diffuse.w) {
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

			// ColliisonBox
			if (temp1->GetComponent<CollisionBox>() != nullptr && temp2->GetComponent<CollisionBox>() != nullptr) {
				if (CollisionBox::OBB(*temp1->GetComponent<CollisionBox>(), *temp2->GetComponent<CollisionBox>())) {
					temp1->OnCollision(temp2);
					temp2->OnCollision(temp1);
					if (temp1->GetComponent<Collider>() != nullptr && temp2->GetComponent<Collider>() != nullptr) {
						Collider::ShiftCollision(temp1, temp2);
					}
				}
			}
			// CollisionSphere
			if (temp1->GetComponent<CollisionSphere>() != nullptr && temp2->GetComponent<CollisionSphere>() != nullptr) {
				if (CollisionSphere::Sphere2Sphere(*temp1->GetComponent<CollisionSphere>(), *temp2->GetComponent<CollisionSphere>())) {
					temp1->OnCollision(temp2);
					temp2->OnCollision(temp1);
					// OBB��p�̍��ɂȂ��Ă��邽�߁A�@�\���Ȃ�
					//if (temp1->GetComponent<Collider>() != nullptr && temp2->GetComponent<Collider>() != nullptr) {
					//	Collider::ShiftCollision(temp1, temp2);
					//}
				}
			}

			// CollisionMesh
			//if (dynamic_cast<GameObjectMeshBase*>(temp1) && dynamic_cast<GameObjectMeshBase*>(temp2)) {
			//	if (CollisionMesh::isMesh2Mesh(
			//		dynamic_cast<GameObjectMeshBase*>(temp1)->m_transform,
			//		dynamic_cast<GameObjectMeshBase*>(temp2)->m_transform)
			//		) {
			//		// �����������ɌĂ΂��֐�
			//		temp1->OnCollision(temp2);
			//		temp2->OnCollision(temp1);
			//		PrintDebugProc("aaaaaaaaaaaaaaaaaaaa\n");
			//	}
			//	PrintDebugProc("bbbbbbbbbbbbbbbbbbbb\n");
			//}
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
	//m_listObject.sort(GameObject3DKdSort);
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

	//--- 3D���f��
	// �e
	std::list<GameObject3D*> shadow;
	//for (auto obj : m_listObject) {
	//	GameObject3D* model = dynamic_cast<GameObject3D*>(obj);
	//	if (model != nullptr && model->m_vs == VS_PROJSHADOW) {
	//		if (model->m_model != E_MODEL::E_MODEL_LAND)
	//			shadow.push_back(model);
	//	}
	//}
	Light::Get()->Shadow(shadow);
	// ���f��
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObject3D*>(obj) != nullptr)
			obj->Draw();
	}
	ShaderManager::GetInstance().BindVS(VS_PROJSHADOW);
	ShaderManager::GetInstance().BindPS(PS_DEPTHSHADOW);
	ShaderManager::GetInstance().BindGS(GS_NORMAL);
	D3DClass::GetInstance().SetBlendState(EBlendState::BS_NONE);	// �u�����h
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);			// �J�����O

	ModelManager::GetInstance().DrawInstanced(E_MODEL_PLAYER);

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);
	D3DClass::GetInstance().SetZBuffer(false);
	buff = m_listObject;
	for (auto obj : buff) {
		if (dynamic_cast<GameObjectMeshBase*>(obj))
			obj->Draw();
	}


	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetZBuffer(false);			// Z�o�b�t�@����
	D3DClass::GetInstance().SetBlendState(EBlendState::BS_ALPHABLEND);	// �A���t�@�������Ȃ�
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