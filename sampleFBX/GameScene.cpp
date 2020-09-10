/**
 * @file	GameManager.cpp
 * @brief	ゲームシーンのマネージャー
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


/**
 * @include
 */
#include "GameScene.h"		// 自身の定義
#include "GameObject3D.h"	// 3Dオブジェクト
#include "GameObjectUI.h"

/**
 * @Component
 */
#include "TPCamera.h"	// TPSカメラ
#include "FPCamera.h"	// FPSカメラ
#include "Sky.h"		// スカイドーム
#include "Player.h"		// プレイヤー
#include "Enemy.h"		// 敵
#include "Collision.h"	// 当たり判定
#include "System.h"		// メモリ監視


static const int MAX_ENEMY = 10;
static const float VAL_ENEMY_POS_Z = 2000.f;
static const float MAX_ENEMY_POS_Z = 3000.f;


int GetRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}


void GameScene::Init() {

	// TPS視点カメラ
	m_empty = new GameObject("TPSCamera");
	m_empty->AddComponent<TPCamera>();
	CCamera::Set(m_empty->GetComponent<TPCamera>());
	m_listObject.push_back(m_empty);


	// スカイドーム
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky");
	m_object3D->AddComponent<CSky>();
	m_listObject.push_back(m_object3D);
	
	// 自機
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player");
	m_object3D->AddComponent<CPlayer>();
	m_object3D->AddComponent<Collision>();
	m_object3D->GetComponent<Collision>()->Init(E_MODEL_PLAYER);
	m_listObject.push_back(m_object3D);

	// 敵機初期化
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

	// push_backの順番でUIの描画のバッファが変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


// EOF