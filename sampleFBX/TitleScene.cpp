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
#include "input.h"
#include "debugproc.h"
#include "GameObject.h"
#include "Camera.h"
#include "System.h"


void TitleScene::Init() {

	GameObject* obj;
	obj = new GameObject;
	obj->AddComponent<CCamera>();
	CCamera::Set(obj->GetComponent<CCamera>());
	m_listObject.push_back(obj);

	// ���O�͍Ō�
	Scene::Init();
}


// EOF