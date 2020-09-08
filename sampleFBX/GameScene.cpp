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
#include "GameScene.h"	// 自身の定義
#include "GameObject.h"	// GameObject

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

	GameObject* obj;	//!< オブジェクト

	// TPS視点カメラ
	obj = new GameObject("TPSCamera");
	obj->AddComponent<TPCamera>();
	CCamera::Set(obj->GetComponent<TPCamera>());
	m_listObject.push_back(obj);


	// スカイドーム
	obj = new GameObject("Sky");
	obj->AddComponent<CSky>();
	m_listObject.push_back(obj);
	
	// 自機
	obj = new GameObject("Player");
	obj->AddComponent<CPlayer>();
	obj->AddComponent<Collision>();
	obj->GetComponent<Collision>()->Init(E_MODEL_PLAYER);
	m_listObject.push_back(obj);

	// 敵機初期化
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



	// push_backの順番でUIの描画のバッファが変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


// EOF