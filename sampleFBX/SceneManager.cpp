#define _CRT_SECURE_NO_WARNINGS

#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "DebugScene.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "D3DClass.h"
#include "input.h"
#include <time.h>
#include "System.h"



SceneManager::SceneManager() : m_scene(new GameScene()) {

}


SceneManager::~SceneManager() {
	delete m_scene;
}


void SceneManager::Init() {
	unsigned int now = (unsigned int)time(0);
	srand(now);

	TextureManager::GetInstance().Initialize();
	ModelManager::GetInstance().Init();
	m_scene->Init();
}


void SceneManager::Uninit() {
	m_scene->Uninit();							// シーン処理の終了
	ModelManager::GetInstance().Uninit();		// モデル解放
	TextureManager::GetInstance().Shutdown();	// テクスチャ解放
}


void SceneManager::Update() {

	if (m_isChange) {
		switch (m_nowScene)
		{
		case E_SCENE::TITLE:
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new TitleScene();
			m_scene->Init();
			break;

		case E_SCENE::GAME:
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new GameScene();
			m_scene->Init();
			break;

		case E_SCENE::RESULT:
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new ResultScene();
			m_scene->Init();
			break;

		case E_SCENE::DEGUG:
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new DebugScene();
			m_scene->Init();
			break;

		}
		m_isChange = false;
	}

	if (Input::isPress(VK_LSHIFT) && Input::isTrigger('1')) {
		LoadScene(E_SCENE::TITLE);
	}
	if (Input::isPress(VK_LSHIFT) && Input::isTrigger('2')) {
		LoadScene(E_SCENE::GAME);
	}
	if (Input::isPress(VK_LSHIFT) && Input::isTrigger('3')) {
		LoadScene(E_SCENE::RESULT);
	}
	if (Input::isPress(VK_LSHIFT) && Input::isTrigger('0')) {
		LoadScene(E_SCENE::DEGUG);
	}

	m_scene->Update();
}


void SceneManager::Draw() {

	m_scene->Draw();

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(false);
}


void SceneManager::LoadScene(E_SCENE scene) {
	m_isChange = true;
	m_nowScene = scene;
}


// EOF