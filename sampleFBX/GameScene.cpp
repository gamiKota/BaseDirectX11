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
#include "TPCamera.h"		// TPS�J����
#include "ggCamera.h"		// gg�J����
#include "Light.h"			// ���C�g
#include "Sky.h"			// �X�J�C�h�[��
#include "PlayerMgr.h"		// �v���C���[�}�l�[�W���[
#include "EnemyFixed.h"		// �Œ�G
#include "EnemyNormal.h"	// �ʏ�G
#include "EnemyElite.h"		// �G���[�g�G
#include "Collision.h"		// �����蔻��
#include "Rigidbody.h"		// �����G���W���@�\
#include "MeshBullet.h"		// ���b�V���e
#include "TargetCtr.h"		// ���b�N�I���}�[�J�[
#include "Material.h"		// �}�e���A��
#include "AreaWall.h"		// ��
// �V�X�e��
#include "System.h"		// �������Ď�


static const int MAX_ENEMY = 1;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;

static const float VAL_WALL_POS = 6000.f;


void GameScene::Init() {

	// ���̃V�[���Ŏg�p����e�N�X�`���̓ǂݍ���
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TREE);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_NUMBER);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TEXT);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_EXPLOSION);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_SUB);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_TARGET);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_WEAPONS);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_FRAME);

	// TPS���_�J����
	m_empty = new GameObject("MainCamera");
#if 0
	CCamera::Set(m_empty->AddComponent<CCamera>());
#else
	CCamera::Set(m_empty->AddComponent<TPCamera>());
#endif
	m_listObject.push_back(m_empty);

	//// �X�J�C�h�[��
	//m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	//m_object3D->AddComponent<SkyDome>();
	//m_object3D->m_isLight = false;
	//m_listObject.push_back(m_object3D);


	// ���C�g(���s����)
	m_object3D = new GameObject3D(E_MODEL_NONE, "MainLight", "MainLight");
	Light::Set(m_object3D->AddComponent<Light>());
	GameObject::Instantiate(m_object3D, float3(), Quaternion::identity, float3() + 20.f);

	//--- �I�u�W�F�N�g�̐���
	// ���@
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
	GameObject::Instantiate(m_object3D, float3(), Quaternion::identity, float3() + 0.5f);
	m_object3D->AddComponent<PlayerMgr>();
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;

	// �G�X�|�[���n�_
	//m_object3D = new GameObject3D(E_MODEL_NONE, "EnemySpawn", "EnemySpawn");
	//GameObject::Instantiate(m_object3D, float3(0.f, 0.f, 200.f), Quaternion::identity, float3() + 15.f);
	//m_object3D->m_ps = E_PS::PS_COLOR;
	//m_object3D->m_material->m_diffuse = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	// �G�@������
	float3 vEnemyPos(0.0f, 0.0f, VAL_ENEMY_POS_Z);
	float3 vPlayerPos = GameObject::FindGameObjectWithTag("Player")->m_transform->m_position;
	for (int i = 0; i < MAX_ENEMY; ++i) {
		m_object3D = new GameObject3D(E_MODEL_PLAYER, "EnemyFixed (" + std::to_string(i) + ")", "Enemy");
		vEnemyPos.x = (float)GetRandom((int)(-1000.f + 30.f), (int)(1000.f - 30.f));
		vEnemyPos.z = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);
		GameObject::Instantiate(m_object3D, vEnemyPos, Quaternion::LookRotation((vPlayerPos - vEnemyPos), float3(0.f, 1.f, 0.f)), float3() + 0.5f);
		m_object3D->AddComponent<EnemyFixed>();
		//m_object3D->m_vs = E_VS::VS_PROJSHADOW;
		//m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;
	}


	//--- �t�B�[���h�̐���
	float3 wallScale = float3(6000.f, 3000.f, 0.1f);
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 0.f, VAL_WALL_POS), Quaternion::AngleAxis(180.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (2)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(-VAL_WALL_POS, 0.f, 0.f), Quaternion::AngleAxis(90.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (3)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 0.f, -VAL_WALL_POS), Quaternion::identity, wallScale);
	m_object3D->AddComponent<AreaWall>();
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (4)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(VAL_WALL_POS, 0.f, 0.f), Quaternion::AngleAxis(-90.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// ��
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (5)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 3000.f, 0.f), Quaternion::AngleAxis(90.f, float3(1.f, 0.f, 0.f)), float3(6000.f, 6000.f, 0.1f));
	m_object3D->AddComponent<AreaWall>();
	// �n��
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	GameObject::Instantiate(m_object3D, float3(0.f, -3000.f, 0.f), Quaternion::identity, float3(3.1f, 3.1f, 3.1f));
	m_object3D->AddComponent<CollisionBox>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Collider>()->m_weight = E_WEIGHT::_LAND;
	m_object3D->GetComponent<CollisionBox>()->m_vCenter = float3(0.f, 0.f, 0.f);
	m_object3D->GetComponent<CollisionBox>()->m_vScale = float3(2000.f, 1.f, 2000.f);
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;


	// push_back�̏��Ԃ�UI�̕`��̕`�揇���ς��
	// �ŏ��ɔw�i�Ȃǂ�UI
	// ����3D�I�u�W�F�N�g
	// �Ō�ɕ\��������UICanvas

	// ���O�͍Ō�
	Scene::Init();
}


// EOF