/**
 * @file	GameManager.cpp
 * @brief	�Q�[���V�[���̃}�l�[�W���[
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


/**
 * @include
 */
// �Q�[���I�u�W�F�N�g
#include "GameScene.h"		// ���g�̒�`
#include "GameObject3D.h"	// 3D�I�u�W�F�N�g
#include "GameObjectUI.h"	// UI�I�u�W�F�N�g
#include "GameObjectMesh.h"	// ���b�V���I�u�W�F�N�g
// �R���|�[�l���g
#include "TPCamera.h"	// TPS�J����
#include "FPCamera.h"	// FPS�J����
#include "Sky.h"		// �X�J�C�h�[��
#include "Player.h"		// �v���C���[
#include "FixedEnemy.h"	// �G
#include "Collision.h"	// �����蔻��
#include "Rigidbody.h"	// �����G���W���@�\
#include "MeshBullet.h"	// ���b�V���e
// �V�X�e��
#include "System.h"		// �������Ď�


static const int MAX_ENEMY = 2;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;


int GetRandom(int min, int max) {
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}


void GameScene::Init() {

	// TPS���_�J����
	m_empty = new GameObject("MainCamera");
#if 0
	m_empty->AddComponent<CCamera>();
	CCamera::Set(m_empty->GetComponent<CCamera>());
#else
	m_empty->AddComponent<TPCamera>();
	CCamera::Set(m_empty->GetComponent<TPCamera>());
#endif
	m_listObject.push_back(m_empty);

	// �X�J�C�h�[��
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_listObject.push_back(m_object3D);
	
	// ���@
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, 0.f);
	m_object3D->AddComponent<PlayerCtr>();
	m_listObject.push_back(m_object3D);

	// �G�@������
	float3 vEnemyPos(0.0f, 0.0f, VAL_ENEMY_POS_Z);
	for (int i = 0; i < MAX_ENEMY; ++i) {
		m_object3D = new GameObject3D(E_MODEL_ENEMY, "FixedEnemy (" + std::to_string(i) + ")", "Enemy");

		vEnemyPos.x = (float)GetRandom((int)(-MAX_MOVE_WIDTH + 30.f), (int)(MAX_MOVE_WIDTH - 30.f));
		vEnemyPos.z = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);

		m_object3D->m_transform->m_position = vEnemyPos;
		m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 180, 0.f);
		m_object3D->AddComponent<FixedEnemy>();
		m_listObject.push_back(m_object3D);
	}

	GameObjectMesh* mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE, "Mesh", "Mesh");
	mesh->m_transform->m_scale = float3(50.f, 100.f, 100.f);
	m_listObject.push_back(mesh);


	mesh = new GameObjectMesh(E_MESH_TYPE::NORMAL, E_TEXTURE::E_TEXTURE_NONE, "Wall", "Wall");
	mesh->m_transform->m_position	= float3(0.f, 5000.f, 5000.f);
	mesh->m_transform->m_scale		= float3(10000.f, 10000.f, 10000.f);
	m_listObject.push_back(mesh);

	// push_back�̏��Ԃ�UI�̕`��̕`�揇���ς��
	// �ŏ��ɔw�i�Ȃǂ�UI
	// ����3D�I�u�W�F�N�g
	// �Ō�ɕ\��������UICanvas

	// ���O�͍Ō�
	Scene::Init();
}


// EOF