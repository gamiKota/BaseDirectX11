#define _CRT_SECURE_NO_WARNINGS

#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "ModelManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "D3DClass.h"
#include "System.h"



SceneManager::SceneManager() : m_scene(new TitleScene()), m_camera(new CCamera()) {

}


SceneManager::~SceneManager() {
	delete m_camera;
	delete m_scene;
}


void SceneManager::Init() {
	m_camera->Init();
	CCamera::Set(m_camera);

	ModelManager::GetInstance().Init();
	
	m_scene->Init();
}


void SceneManager::Uninit() {

	m_scene->Uninit();
	
	ModelManager::GetInstance().Uninit();

	m_camera->Uninit();
}


void SceneManager::Update() {

	if (m_isChange) {
		switch (m_nowScene)
		{
		case E_SCENE::TITLE:
			CCamera::Set(m_camera);
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new TitleScene();
			m_scene->Init();
			break;

		case E_SCENE::GAME:
			CCamera::Set(m_camera);
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new GameScene();
			m_scene->Init();
			break;

		case E_SCENE::RESULT:
			CCamera::Set(m_camera);
			m_scene->Uninit();
			SAFE_DELETE(m_scene);
			m_scene = new ResultScene();
			m_scene->Init();
			break;

		}
		m_isChange = false;
	}
	CCamera::Get()->Update();
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