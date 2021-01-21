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
#include "Light.h"		// ���C�g
#include "Sky.h"		// �X�J�C�h�[��
#include "PlayerMgr.h"	// �v���C���[�}�l�[�W���[
#include "EnemyFixed.h"	// �Œ�G
#include "Collision.h"	// �����蔻��
#include "Rigidbody.h"	// �����G���W���@�\
#include "MeshBullet.h"	// ���b�V���e
#include "TargetCtr.h"	// ���b�N�I���}�[�J�[
// �V�X�e��
#include "System.h"	// �������Ď�


static const int MAX_ENEMY = 1;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;

static const float VAL_WALL_POS = 6000.f;


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
	CCamera::Set(m_empty->AddComponent<CCamera>());
#else
	CCamera::Set(m_empty->AddComponent<TPCamera>());
#endif
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

	//--- �I�u�W�F�N�g�̐���
	// ���@
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
	m_object3D->m_transform->m_position = float3(0.f, 200.f, 200.f);
	m_object3D->AddComponent<PlayerMgr>();
	//m_object3D->m_shader = E_SHADER_TOON;
	m_listObject.push_back(m_object3D);

	// �G�@������
	float3 vEnemyPos(0.0f, 0.0f, VAL_ENEMY_POS_Z);
	for (int i = 0; i < MAX_ENEMY; ++i) {
		m_object3D = new GameObject3D(E_MODEL_ENEMY, "EnemyFixed (" + std::to_string(i) + ")", "Enemy");

		vEnemyPos.x = (float)GetRandom((int)(-1000.f + 30.f), (int)(1000.f - 30.f));
		vEnemyPos.z = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);

		m_object3D->m_transform->m_position = vEnemyPos;
		//m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 180, 0.f);
		//m_object3D->m_transform->m_scale = float3(2.f, 2.f, 2.f);
		m_object3D->m_transform->m_scale = float3(0.5f, 0.5f, 0.5f);
		m_object3D->AddComponent<EnemyFixed>();
		//m_object3D->m_shader = E_SHADER_TOON;
		m_listObject.push_back(m_object3D);
	}

	// �r���{�[�h
	//m_mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE, "Mesh", "Mesh");
	//m_mesh->m_transform->m_position = float3(0.f, 0.f, 400.f);
	//m_mesh->m_transform->m_scale = float3(50.f, 50.f, 50.f);
	////m_mesh->m_mesh.light = false;
	//m_listObject.push_back(m_mesh);

	// �{�b�N�X
	m_object3D = new GameObject3D(E_MODEL_NONE, "box", "box");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, 100.f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>();
	m_object3D->m_transform->m_scale = float3(20.f, 20.f, 100.f);
	m_object3D->m_material.Kd = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_listObject.push_back(m_object3D);


	//--- �t�B�[���h�̐���
	float3 scale = float3(6000.f, 3000.f, 0.1f);
	TFbxMaterial material;
	material.Ka.w = 0.f;
	material.Kd = XMFLOAT4(1.f, 1.f, 1.f, 0.f);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, VAL_WALL_POS);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(180.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (2)", "AreaWall");
	m_object3D->m_transform->m_position = float3(-VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(90.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (3)", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, -VAL_WALL_POS);
	//m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 0.f, 0.f);
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (4)", "AreaWall");
	m_object3D->m_transform->m_position = float3(VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(-90.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (5)", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 3000.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(90.f, float3(1.f, 0.f, 0.f));
	m_object3D->m_transform->m_scale = float3(6000.f, 6000.f, 0.1f);
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// �n��
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	m_object3D->m_transform->m_position = float3(0.f, -3000.f, 0.f);
	m_object3D->m_transform->m_scale = float3(3.1f, 3.1f, 3.1f);
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
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