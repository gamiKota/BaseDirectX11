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
#include "GameScene.h"	// ���g�̒�`
#include "GameObject.h"	// GameObject

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

	GameObject* obj;	//!< �I�u�W�F�N�g

	// TPS���_�J����
	obj = new GameObject("TPSCamera");
	obj->AddComponent<TPCamera>();
	CCamera::Set(obj->GetComponent<TPCamera>());
	m_listObject.push_back(obj);


	// �X�J�C�h�[��
	obj = new GameObject("Sky");
	obj->AddComponent<CSky>();
	m_listObject.push_back(obj);
	
	// ���@
	obj = new GameObject("Player");
	obj->AddComponent<CPlayer>();
	obj->AddComponent<Collision>();
	obj->GetComponent<Collision>()->Init(E_MODEL_PLAYER);
	m_listObject.push_back(obj);

	// �G�@������
	XMFLOAT3 vEnemyPos(0.0f, 0.0f, VAL_ENEMY_POS_Z);
	for (int i = 0; i < MAX_ENEMY; ++i) {
		obj = new GameObject("Enemy (" + std::to_string(i) + ")", "Enemy");

		vEnemyPos.x = (float)GetRandom((int)(-MAX_MOVE_WIDTH + 30.f), (int)(MAX_MOVE_WIDTH - 30.f));
		vEnemyPos.z = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);

		obj->m_transform->m_position = vEnemyPos;
		obj->m_transform->m_rotate.y = 180;
		obj->AddComponent<CEnemy>();
		obj->AddComponent<Collision>();
		obj->GetComponent<Collision>()->Init(E_MODEL_ENEMY);
		m_listObject.push_back(obj);
	}



	// push_back�̏��Ԃ�UI�̕`��̃o�b�t�@���ς��
	// �ŏ��ɔw�i�Ȃǂ�UI
	// ����3D�I�u�W�F�N�g
	// �Ō�ɕ\��������UICanvas

	// ���O�͍Ō�
	Scene::Init();
}


// EOF