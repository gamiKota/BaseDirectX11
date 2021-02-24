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
#include "TPCamera.h"		// TPSカメラ
#include "ggCamera.h"		// ggカメラ
#include "Light.h"			// ライト
#include "Sky.h"			// スカイドーム
#include "PlayerMgr.h"		// プレイヤーマネージャー
#include "EnemyFixed.h"		// 固定敵
#include "EnemyNormal.h"	// 通常敵
#include "EnemyElite.h"		// エリート敵
#include "Collision.h"		// 当たり判定
#include "Rigidbody.h"		// 物理エンジン機能
#include "MeshBullet.h"		// メッシュ弾
#include "TargetCtr.h"		// ロックオンマーカー
#include "Material.h"		// マテリアル
#include "AreaWall.h"		// 壁
// システム
#include "System.h"		// メモリ監視


static const int MAX_ENEMY = 1;
static const float VAL_ENEMY_POS_Z = 800.f;
static const float MAX_ENEMY_POS_Z = 1000.f;

static const float VAL_WALL_POS = 6000.f;


void GameScene::Init() {

	// このシーンで使用するテクスチャの読み込み
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

	// TPS視点カメラ
	m_empty = new GameObject("MainCamera");
#if 0
	CCamera::Set(m_empty->AddComponent<CCamera>());
#else
	CCamera::Set(m_empty->AddComponent<TPCamera>());
#endif
	m_listObject.push_back(m_empty);

	//// スカイドーム
	//m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	//m_object3D->AddComponent<SkyDome>();
	//m_object3D->m_isLight = false;
	//m_listObject.push_back(m_object3D);


	// ライト(平行光源)
	m_object3D = new GameObject3D(E_MODEL_NONE, "MainLight", "MainLight");
	Light::Set(m_object3D->AddComponent<Light>());
	GameObject::Instantiate(m_object3D, float3(), Quaternion::identity, float3() + 20.f);

	//--- オブジェクトの生成
	// 自機
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
	GameObject::Instantiate(m_object3D, float3(), Quaternion::identity, float3() + 0.5f);
	m_object3D->AddComponent<PlayerMgr>();
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;

	// 敵スポーン地点
	//m_object3D = new GameObject3D(E_MODEL_NONE, "EnemySpawn", "EnemySpawn");
	//GameObject::Instantiate(m_object3D, float3(0.f, 0.f, 200.f), Quaternion::identity, float3() + 15.f);
	//m_object3D->m_ps = E_PS::PS_COLOR;
	//m_object3D->m_material->m_diffuse = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	// 敵機初期化
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


	//--- フィールドの生成
	float3 wallScale = float3(6000.f, 3000.f, 0.1f);
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 0.f, VAL_WALL_POS), Quaternion::AngleAxis(180.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (2)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(-VAL_WALL_POS, 0.f, 0.f), Quaternion::AngleAxis(90.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (3)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 0.f, -VAL_WALL_POS), Quaternion::identity, wallScale);
	m_object3D->AddComponent<AreaWall>();
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (4)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(VAL_WALL_POS, 0.f, 0.f), Quaternion::AngleAxis(-90.f, float3(0.f, 1.f, 0.f)), wallScale);
	m_object3D->AddComponent<AreaWall>();
	// 壁
	m_object3D = new GameObject3D(E_MODEL_NONE, "AreaWall (5)", "AreaWall");
	GameObject::Instantiate(m_object3D, float3(0.f, 3000.f, 0.f), Quaternion::AngleAxis(90.f, float3(1.f, 0.f, 0.f)), float3(6000.f, 6000.f, 0.1f));
	m_object3D->AddComponent<AreaWall>();
	// 地面
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	GameObject::Instantiate(m_object3D, float3(0.f, -3000.f, 0.f), Quaternion::identity, float3(3.1f, 3.1f, 3.1f));
	m_object3D->AddComponent<CollisionBox>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Collider>()->m_weight = E_WEIGHT::_LAND;
	m_object3D->GetComponent<CollisionBox>()->m_vCenter = float3(0.f, 0.f, 0.f);
	m_object3D->GetComponent<CollisionBox>()->m_vScale = float3(2000.f, 1.f, 2000.f);
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;


	// push_backの順番でUIの描画の描画順が変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


// EOF