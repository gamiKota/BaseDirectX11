﻿/**
 * @file Player.cpp
 */


/**
 * @include
 */
#include "PlayerMgr.h"
#include <math.h>
#include "GameObject3D.h"
#include "input.h"
#include "debugproc.h"
#include "Bullet.h"
#include "Status.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "PlayerState.h"
#include "PlayerOperation.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "System.h"


/**
 * @abridgement namespace
 */
using namespace DirectX;


void PlayerMgr::Start() {
	// Characterクラスの初期化
	Character::Init();

	// プレイヤーに必要なコンポーネントの追加
	m_state = m_gameObject->AddComponent<PlayerState>();
	m_operate = m_gameObject->AddComponent<PlayerOperation>();

	// 変数の初期化
	m_rigidbody->m_weight = E_WEIGHT::_2;
}


void PlayerMgr::Update() {
	// 主にデバッグ用
	//Tween tween;
	//float3 end = float3(20.f, 10.f, 30.f);
	//if (Input::isTrigger('M')) {
	//	m_transform->DOMove(end, 2.f);
	//}
	//Quaternion q1 = Quaternion::AngleAxis(-90.f,   float3(1.f, 0.f, 0.f));
	//Quaternion q2 = Quaternion::AngleAxis(-90.f,   float3(0.f, 1.f, 0.f));
	//Quaternion q3 = Quaternion::AngleAxis(-90.f,   float3(0.f, 0.f, 1.f));
	//Quaternion q_1 = q1 * q2 * q3;
	//m_transform->m_rotate = q_1;
}


void PlayerMgr::Uninit() {
}


void PlayerMgr::OnCollision(GameObject* obj) {
	//PrintDebugProc("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
}


void PlayerMgr::SetImGuiVal() {
#if _DEBUG
#endif
}


// EOF