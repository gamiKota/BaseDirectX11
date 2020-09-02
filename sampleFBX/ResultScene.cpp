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
#include "ResultScene.h"		// 自身のヘッダー
#include "D3DClass.h"
#include "GameObject.h"

#include "debugproc.h"
#include "input.h"

#include "System.h"


ResultScene::ResultScene() : Scene() {
}


ResultScene::~ResultScene() {
	Scene::~Scene();
}


void ResultScene::Init() {
	// お前は最後
	Scene::Init();
}


void ResultScene::Uninit() {
	Scene::Uninit();
}

void ResultScene::Update() {
	PrintDebugProc("ResultScene");
	if (Input::isTrigger(VK_SPACE)) {
		SceneManager::GetInstance().LoadScene(E_SCENE::TITLE);
	}
	Scene::Update();
}


void ResultScene::Draw() {
	// シーンの描画
	Scene::Draw();
}


// EOF