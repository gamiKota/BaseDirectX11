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
#include "GameScene.h"		// ���g�̒�`
#include "GameObject3D.h"	// 3D�I�u�W�F�N�g
#include "GameObjectUI.h"

/**
 * @Component
 */
#include "TPCamera.h"	// TPS�J����
#include "FPCamera.h"	// FPS�J����
#include "Sky.h"		// �X�J�C�h�[��
#include "Player.h"		// �v���C���[
#include "Enemy.h"		// �G
#include "Collision.h"	// �����蔻��
#include "System.h"		// �������Ď�


static const int MAX_ENEMY = 10;
static const float VAL_ENEMY_POS_Z = 2000.f;
static const float MAX_ENEMY_POS_Z = 3000.f;


int GetRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}


void GameScene::Init() {

	// TPS���_�J����
	m_empty = new GameObject("TPSCamera");
	m_empty->AddComponent<TPCamera>();
	CCamera::Set(m_empty->GetComponent<TPCamera>());
	m_listObject.push_back(m_empty);


	// �X�J�C�h�[��
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky");
	m_object3D->AddComponent<CSky>();
	m_listObject.push_back(m_object3D);
	
	// ���@
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player");
	m_object3D->AddComponent<CPlayer>();
	m_object3D->AddComponent<Collision>();
	m_object3D->GetComponent<Collision>()->Init(E_MODEL_PLAYER);
	m_listObject.push_back(m_object3D);

	// �G�@������
	float3 vEnemyPos(0.0f, 0.0f, VAL_ENEMY_POS_Z);
	for (int i = 0; i < MAX_ENEMY; ++i) {
		m_object3D = new GameObject3D(E_MODEL_ENEMY, "Enemy (" + std::to_string(i) + ")", "Enemy");

		vEnemyPos.x = (float)GetRandom((int)(-MAX_MOVE_WIDTH + 30.f), (int)(MAX_MOVE_WIDTH - 30.f));
		vEnemyPos.z = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);

		m_object3D->m_transform->m_position = vEnemyPos;
		m_object3D->m_transform->m_rotate.y = 180;
		m_object3D->AddComponent<CEnemy>();
		m_object3D->AddComponent<Collision>();
		m_object3D->GetComponent<Collision>()->Init(E_MODEL_ENEMY);
		m_listObject.push_back(m_object3D);
	}

	//m_UI = new GameObjectUI(E_LAYER::UI, E_TEXTURE_NONE, "UI", "UI");
	//m_listObject.push_back(m_UI);

	// push_back�̏��Ԃ�UI�̕`��̃o�b�t�@���ς��
	// �ŏ��ɔw�i�Ȃǂ�UI
	// ����3D�I�u�W�F�N�g
	// �Ō�ɕ\��������UICanvas

	// ���O�͍Ō�
	Scene::Init();
}


// EOF