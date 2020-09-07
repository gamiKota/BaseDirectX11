/**
 * @file	GameManager.cpp
 * @brief	ゲームシーンのマネージャー
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "GameScene.h"		// 自身のヘッダー
#include "D3DClass.h"
#include "GameObject.h"

#include "TPCamera.h"
#include "FPCamera.h"
#include "Player.h"
#include "Sky.h"
#include "Collision.h"
#include "Enemy.h"
#include "Number.h"
#include "Score.h"

#include "debugproc.h"
#include "input.h"

#include "System.h"


static const int MAX_ENEMY = 10;
static const float VAL_ENEMY_POS_Z = 2000.f;
static const float MAX_ENEMY_POS_Z = 3000.f;


CTPCamera					g_cameraTP;
CFPCamera					g_cameraFP;


int GetRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}


GameScene::GameScene() : Scene() {
}


GameScene::~GameScene() {
	Scene::~Scene();
}


void GameScene::Init() {

	HRESULT hr;

	// カメラ初期化
	hr = g_cameraTP.Init();
	hr = g_cameraFP.Init();
	CCamera::Set(&g_cameraTP);

	InitNumber();

	GameObject* obj;

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
	XMFLOAT4X4 mWorld(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, VAL_ENEMY_POS_Z, 1.0f);
	for (int i = 0; i < MAX_ENEMY; ++i) {
		obj = new GameObject("Enemy (" + std::to_string(i) + ")", "Enemy");

		//float size = (float)GetRandom(1, 10) * 0.1f;
		//mWorld._11 = size;
		//mWorld._22 = size;
		//mWorld._33 = size;

		mWorld._41 = (float)GetRandom((int)(-MAX_MOVE_WIDTH + 30.f), (int)(MAX_MOVE_WIDTH - 30.f));
		mWorld._43 = (float)GetRandom((int)VAL_ENEMY_POS_Z, (int)MAX_ENEMY_POS_Z);

		obj->m_transform.m_world = mWorld;
		obj->AddComponent<CEnemy>();
		obj->AddComponent<Collision>();
		obj->GetComponent<Collision>()->Init(E_MODEL_ENEMY);
		m_listObject.push_back(obj);
	}

	obj = new GameObject("Score");
	obj->AddComponent<Score>();
	m_listUI.push_back(obj);

	// push_backの順番でUIの描画のバッファが変わる
	// 最初に背景などのUI
	// 次に3Dオブジェクト
	// 最後に表示したいUICanvas

	// お前は最後
	Scene::Init();
}


void GameScene::Uninit() {
	Scene::Uninit();
	

	UninitNumber();
	// カメラ終了処理
	g_cameraFP.Uninit();
	g_cameraTP.Uninit();
}


void GameScene::Update() {

	// カメラ更新
	if (Input::isTrigger('1')) {
		CCamera::Set(SceneManager::GetInstance().m_camera);
	}
	if (Input::isTrigger('2')) {
		CCamera::Set(&g_cameraFP);
	}
	if (Input::isTrigger('3')) {
		CCamera::Set(&g_cameraTP);
	}

	bool isClear = true;
	for (auto obj : m_listObject) {
		if (obj->GetComponent<CEnemy>() != nullptr) {	// 敵が一機でも生きていたら
			isClear = false;
		}
	}
	if (isClear) {
		SceneManager::GetInstance().LoadScene(E_SCENE::RESULT);
	}

	Scene::Update();
}


void GameScene::Draw() {

	// シーンの描画
	Scene::Draw();
}


// EOF