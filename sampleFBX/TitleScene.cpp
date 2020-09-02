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
#include "TitleScene.h"		// ���g�̃w�b�_�[
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
	// ���O�͍Ō�
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
	// �V�[���̕`��
	Scene::Draw();
}


// EOF