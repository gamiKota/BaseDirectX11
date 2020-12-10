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
#include "Light.h"
#include "System.h"


void TitleScene::Init() {

	// カメラ
	m_empty = new GameObject;
	CCamera::Set(m_empty->AddComponent<CCamera>());
	m_listObject.push_back(m_empty);

	// ライト(平行光源)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// お前は最後
	Scene::Init();
}


// EOF