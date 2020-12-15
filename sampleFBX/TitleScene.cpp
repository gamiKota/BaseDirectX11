/**
 * @file	GameManager.cpp
 * @brief	�Q�[���V�[���̃}�l�[�W���[
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


 //*****************************************************************************
 // �C���N���[�h��
 //*****************************************************************************
#include "TitleScene.h"		// ���g�̃w�b�_�[
// �Q�[���I�u�W�F�N�g
#include "debugproc.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "GameObjectMesh.h"
#include "WaterSurface.h"
// �R���|�[�l���g
#include "Camera.h"
#include "Light.h"
#include "Sky.h"
// �V�X�e��
#include "System.h"


void TitleScene::Init() {

	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TREE);

	// �J����
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<CCamera>());
	m_listObject.push_back(m_empty);

	// ���C�g(���s����)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// �X�J�C�h�[��
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_object3D->m_isLight = false;
	m_listObject.push_back(m_object3D);

	// ����
	WaterSurface* waterSurface = new WaterSurface("WaterSurface", "WaterSurface");
	waterSurface->m_mesh.light = false;
	m_listObject.push_back(waterSurface);

	// ����
	m_mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE,  "Mesh", "mesh");
	m_mesh->m_transform->m_position.y = -100.f;
	m_mesh->m_mesh.light = false;
	m_listObject.push_back(m_mesh);

	// ���O�͍Ō�
	Scene::Init();
}


// EOF