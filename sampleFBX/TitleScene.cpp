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

	// お前は最後
	Scene::Init();
}


// EOF