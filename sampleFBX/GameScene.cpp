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
// ゲームオブジェクト
#include "GameScene.h"		// 自身の定義
#include "GameObject3D.h"	// 3Dオブジェクト
#include "GameObjectUI.h"	// UIオブジェクト
#include "GameObjectMesh.h"	// メッシュオブジェクト
// コンポーネント
#include "TPCamera.h"	// TPSカメラ
#include "FPCamera.h"	// FPSカメラ
#include "Sky.h"		// スカイドーム
#include "Player.h"		// プレイヤー
#include "FixedEnemy.h"	// 敵
#include "Collision.h"	// 当たり判定
#include "Rigidbody.h"	// 物理エンジン機能
#include "MeshBullet.h"	// メッシュ弾
// システム
#include "System.h"		// メモリ監視


static const int MAX_ENEMY = 2;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;


int GetRandom(int min, int max) {
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}


void GameScene::Init() {

	// TPS視点カメラ
	m_empty = new GameObject("MainCamera");
#if 0
	m_empty->AddComponent<CCamera>();
	CCamera::Set(m_empty->GetComponent<CCamera>());
#else
	m_empty->AddComponent<TPCamera>();
	CCamera::Set(m_empty->GetComponent<TPCamera>());
#endif
	m_listObject.push_back(m_empty);

	// スカイドーム
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_listObject.push_back(m_object3D);
	
	// 自機
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, 0.f);
	m_object3D->AddComponent<PlayerCtr>();
	m_listObject.push_back(m_object3D);

	// 敵機初期化
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

	// push_backの順番でUIの描画の描画順が変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


// EOF