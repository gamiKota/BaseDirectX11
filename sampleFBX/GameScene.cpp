/**
 * @file	GameManager.cpp
 * @brief	ゲームシーンのマネージャー
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


/*
メモ：

ターゲット表示
	ターゲット表示はモデルに追従してるけど、UIの様にモデルより前に描画される
	ターゲット表示の右下に並ぶようにターゲットのHPバー
	ミッションにかかわる重要ターゲットの場合、HPバーの下に「TARGET」と表示される
	ターゲットにしてない他的も小さな表示で出る
	画面外に出るとカメラ枠の横に出る(攻撃してると強調表示される)
　色：
	自分を狙ってる敵は赤色
	自分を狙ってない敵は黄色
	(怯み状態になると緑色)
	味方は青色

左下UI
	TH ダッシュのスタミナゲージ
	SH シールドのダメージ蓄積ゲージ
右下UI
	WEAPONS 通常弾の残り弾数
	HP 体力ゲージ
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
#include "Light.h"		// ライト
#include "Sky.h"		// スカイドーム
#include "PlayerMgr.h"	// プレイヤーマネージャー
#include "EnemyFixed.h"	// 固定敵
#include "Collision.h"	// 当たり判定
#include "Rigidbody.h"	// 物理エンジン機能
#include "MeshBullet.h"	// メッシュ弾
#include "TargetCtr.h"	// ロックオンマーカー
// システム
#include "System.h"	// メモリ監視


static const int MAX_ENEMY = 1;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;

static const float VAL_WALL_POS = 6000.f;


void GameScene::Init() {

	// このシーンで使用するテクスチャの読み込み
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TREE);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_NUMBER);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_EXPLOSION);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN);
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_SUB);

	// TPS視点カメラ
	m_empty = new GameObject("MainCamera");
#if 0
	CCamera::Set(m_empty->AddComponent<CCamera>());
#else
	CCamera::Set(m_empty->AddComponent<TPCamera>());
#endif
	m_listObject.push_back(m_empty);

	// ライト(平行光源)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// スカイドーム
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_object3D->m_isLight = false;
	m_listObject.push_back(m_object3D);

	//--- オブジェクトの生成
	// 自機
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
	m_object3D->m_transform->m_position = float3(0.f, 200.f, 200.f);
	m_object3D->AddComponent<PlayerMgr>();
	//m_object3D->m_shader = E_SHADER_TOON;
	m_listObject.push_back(m_object3D);

	// 敵機初期化
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

	// ビルボード
	//m_mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE, "Mesh", "Mesh");
	//m_mesh->m_transform->m_position = float3(0.f, 0.f, 400.f);
	//m_mesh->m_transform->m_scale = float3(50.f, 50.f, 50.f);
	////m_mesh->m_mesh.light = false;
	//m_listObject.push_back(m_mesh);

	// ボックス
	m_object3D = new GameObject3D(E_MODEL_NONE, "box", "box");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, 100.f);
	m_object3D->AddComponent<Collision>();
	m_object3D->AddComponent<Rigidbody>();
	m_object3D->m_transform->m_scale = float3(20.f, 20.f, 100.f);
	m_object3D->m_material.Kd = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_listObject.push_back(m_object3D);


	//--- フィールドの生成
	float3 scale = float3(6000.f, 3000.f, 0.1f);
	TFbxMaterial material;
	material.Ka.w = 0.f;
	material.Kd = XMFLOAT4(1.f, 1.f, 1.f, 0.f);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, VAL_WALL_POS);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(180.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (2)", "AreaWall");
	m_object3D->m_transform->m_position = float3(-VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(90.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (3)", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 0.f, -VAL_WALL_POS);
	//m_object3D->m_transform->m_rotate = Quaternion::Euler(0.f, 0.f, 0.f);
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (4)", "AreaWall");
	m_object3D->m_transform->m_position = float3(VAL_WALL_POS, 0.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(-90.f, float3(0.f, 1.f, 0.f));
	m_object3D->m_transform->m_scale = scale;
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (5)", "AreaWall");
	m_object3D->m_transform->m_position = float3(0.f, 3000.f, 0.f);
	m_object3D->m_transform->m_rotation = Quaternion::AngleAxis(90.f, float3(1.f, 0.f, 0.f));
	m_object3D->m_transform->m_scale = float3(6000.f, 6000.f, 0.1f);
	m_object3D->m_material = material;
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
	m_listObject.push_back(m_object3D);
	// 地面
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	m_object3D->m_transform->m_position = float3(0.f, -3000.f, 0.f);
	m_object3D->m_transform->m_scale = float3(3.1f, 3.1f, 3.1f);
	m_object3D->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_LAND;
	m_listObject.push_back(m_object3D);


	// push_backの順番でUIの描画の描画順が変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


// EOF