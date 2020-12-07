/**
 * @file	GameManager.cpp
 * @brief	�Q�[���V�[���̃}�l�[�W���[
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


/*
�����F

�^�[�Q�b�g�\��
	�^�[�Q�b�g�\���̓��f���ɒǏ]���Ă邯�ǁAUI�̗l�Ƀ��f�����O�ɕ`�悳���
	�^�[�Q�b�g�\���̉E���ɕ��Ԃ悤�Ƀ^�[�Q�b�g��HP�o�[
	�~�b�V�����ɂ������d�v�^�[�Q�b�g�̏ꍇ�AHP�o�[�̉��ɁuTARGET�v�ƕ\�������
	�^�[�Q�b�g�ɂ��ĂȂ����I�������ȕ\���ŏo��
	��ʊO�ɏo��ƃJ�����g�̉��ɏo��(�U�����Ă�Ƌ����\�������)
�@�F�F
	������_���Ă�G�͐ԐF
	������_���ĂȂ��G�͉��F
	(���ݏ�ԂɂȂ�ƗΐF)
	�����͐F

����UI
	TH �_�b�V���̃X�^�~�i�Q�[�W
	SH �V�[���h�̃_���[�W�~�σQ�[�W
�E��UI
	WEAPONS �ʏ�e�̎c��e��
	HP �̗̓Q�[�W
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
#include "TargetCtr.h"	// ���b�N�I���}�[�J�[
// �V�X�e��
#include "System.h"	// �������Ď�


static const int MAX_ENEMY = 2;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;

static const float VAL_WALL_POS = 6000.f;


int GetRandom(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}


void GameScene::Init() {

	// ���̃V�[���Ŏg�p����e�N�X�`���̓ǂݍ���
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TREE);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_NUMBER);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_EXPLOSION);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_SUB);

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
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_listObject.push_back(m_object3D);

	//--- �I�u�W�F�N�g�̐���
	// ���@
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
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

	// �r���{�[�h
	GameObjectMesh* mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE, "Mesh", "Mesh");
	mesh->m_transform->m_position = float3(0.f, 0.f, 400.f);
	mesh->m_transform->m_scale = float3(50.f, 50.f, 50.f);
	m_listObject.push_back(mesh);

	// ���b�N�I���}�[�J�[
	m_UI = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK);
	m_UI->AddComponent<TargetCtr>();
	m_listObject.push_back(m_UI);


	//--- �t�B�[���h�̐���
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, VAL_WALL_POS);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 180.f, 0.f);
	m_object3D->m_transform->m_scale = float3(50.f, 50.f, 0.1f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(-VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 90.f, 0.f);
	m_object3D->m_transform->m_scale = float3(50.f, 50.f, 0.1f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, -VAL_WALL_POS);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 0.f, 0.f);
	m_object3D->m_transform->m_scale = float3(50.f, 50.f, 0.1f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, -90.f, 0.f);
	m_object3D->m_transform->m_scale = float3(50.f, 50.f, 0.1f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 2250.f, 0.f);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(90.f, 0.f, 0.f);
	m_object3D->m_transform->m_scale = float3(150.f, 150.f, 0.1f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// �n��
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	m_object3D->m_transform->m_position = float3(0.f, -2250.f, 0.f);
	m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 0.f, 0.f);
	m_object3D->m_transform->m_scale = float3(3.f, 3.f, 3.f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_LAND;
	m_listObject.push_back(m_object3D);

	// push_back�̏��Ԃ�UI�̕`��̕`�揇���ς��
	// �ŏ��ɔw�i�Ȃǂ�UI
	// ����3D�I�u�W�F�N�g
	// �Ō�ɕ\��������UICanvas

	// ���O�͍Ō�
	Scene::Init();
}


// EOF