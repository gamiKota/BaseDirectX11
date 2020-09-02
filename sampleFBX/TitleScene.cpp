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
#include "TitleScene.h"		// 自身のヘッダー
#include "D3DClass.h"
#include "GameObject.h"

#include "debugproc.h"
#include "input.h"

#include "System.h"


TitleScene::TitleScene() : Scene() {
}

TitleScene::~TitleScene() {
	Scene::~Scene();
}


void TitleScene::Init() {
	// お前は最後
	Scene::Init();
}


void TitleScene::Uninit() {
	Scene::Uninit();
}

void TitleScene::Update() {
	PrintDebugProc("TitleScene");
	if (Input::isTrigger(VK_SPACE)) {
		SceneManager::GetInstance().LoadScene(E_SCENE::GAME);
	}
	Scene::Update();
}


void TitleScene::Draw() {
	// シーンの描画
	Scene::Draw();
}


// EOF