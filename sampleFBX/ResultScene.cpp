/**
 * @file	GameManager.cpp
 * @brief	�Q�[���V�[���̃}�l�[�W���[
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


 //*****************************************************************************
 // �C���N���[�h��
 //*****************************************************************************
#include "ResultScene.h"		// ���g�̃w�b�_�[
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
	// ���O�͍Ō�
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
	// �V�[���̕`��
	Scene::Draw();
}


// EOF