#include "SelectScene.h"
// ゲームオブジェクト
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "GameObjectMesh.h"
// コンポーネント
#include "Camera.h"
#include "Light.h"
#include "Sky.h"
// システム
#include "System.h"


void SelectScene::Init() {

	// このシーンで使用するテクスチャの読み込み
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_SELECT01);

	// カメラ
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<CCamera>());
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

	// 殲滅作戦ロゴ
	m_UI = new GameObjectUI(E_LAYER::UI, E_TEXTURE_SELECT01, "button01", "button");
	GameObject::Instantiate(m_UI, float3(), Quaternion::identity, float3(200.f, 70.f, 0.f));

	// お前は最後
	Scene::Init();
}


// EOF