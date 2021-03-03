/**
 * @file	TitleScene.cpp
 * @brief	タイトル
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


 //*****************************************************************************
 // インクルード部
 //*****************************************************************************
#include "TitleScene.h"		// 自身のヘッダー
// ゲームオブジェクト
#include "debugproc.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "GameObjectMesh.h"
#include "WaterSurface.h"
// コンポーネント
#include "Camera.h"
#include "ggCamera.h"
#include "Light.h"
#include "Sky.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "PlayerMgr.h"
#include "TitlePlayer.h"
// システム
#include "System.h"


void TitleScene::Init() {

	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_TITLE);
	//TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_WATER_SURFACE);

	// カメラ
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<CCamera>());
	GameObject::Instantiate(m_empty, float3(-3000.f, -2724.f, -3000.f), Quaternion::Euler(25.f, 45.f, 0.f));

	// ライト(平行光源)
	m_object3D = new GameObject3D(E_MODEL_NONE, "MainLight", "MainLight");
	Light::Set(m_object3D->AddComponent<Light>());
	GameObject::Instantiate(m_object3D, float3(-4000.f, 4000.f, -4000.f), Quaternion::identity, float3() + 20.f);

	// スカイドーム
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_object3D->m_isLight = false;
	m_listObject.push_back(m_object3D);

	// 殲滅作戦ロゴ
	m_UI = new GameObjectUI(E_LAYER::UI, E_TEXTURE_TITLE, "title", "title");
	GameObject::Instantiate(m_UI, float3(), Quaternion::identity, float3(200.f, 70.f, 0.f));

	// 自機
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "Player", "Player");
	GameObject::Instantiate(m_object3D, float3(-2675.f, -2937.f, -3325.f), Quaternion::identity, float3() + 0.5f);
	//m_object3D->AddComponent<PlayerMgr>();
	m_object3D->AddComponent<TitlePlayer>()->m_isCreate = false;
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;

	// 地面
	m_object3D = new GameObject3D(E_MODEL_LAND, "Land", "Land");
	GameObject::Instantiate(m_object3D, float3(0.f, -3000.f, 0.f), Quaternion::identity, float3(3.1f, 3.1f, 3.1f));
	m_object3D->AddComponent<CollisionBox>()->m_selfTag.push_back("Area");
	m_object3D->AddComponent<Collider>()->m_weight = E_WEIGHT::_LAND;
	m_object3D->GetComponent<CollisionBox>()->m_vCenter = float3(0.f, 0.f, 0.f);
	m_object3D->GetComponent<CollisionBox>()->m_vScale = float3(2000.f, 1.f, 2000.f);
	m_object3D->m_vs = E_VS::VS_PROJSHADOW;
	m_object3D->m_ps = E_PS::PS_DEPTHSHADOW;

	// お前は最後
	Scene::Init();


	// ちんこ
}


// EOF