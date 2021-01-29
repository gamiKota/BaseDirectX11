/**
 * @file Player.cpp
 */


/**
 * @include
 */
#include "PlayerMgr.h"
#include "Transform.h"
#include "GameObject3D.h"
#include "debugproc.h"
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
	m_rigidbody->m_weight = E_WEIGHT::_3;

	m_collision->m_vScale = float3(140.f, 40.f, 100.f);
	m_collision->m_vCenter = float3(0.f, 0.f, 0.f);

	//m_transform->DOMove(float3(0.f, 0.f, 0.f), 2.f)->OnComplete([state = m_state]() {
	//	state->SetStateActive(PLAYER_STATE::TARGET_ON, true);
	//});
}



void PlayerMgr::Update() {
	// 主にデバッグ用
	//float3 end = float3(20.f, 10.f, 30.f);
	//if (Input::isTrigger('M')) {
	//	m_transform->DOMove(end, 2.f)->OnComplete([state = m_state]() {
	//		state->SetStateActive(PLAYER_STATE::TARGET_ON, true);
	//	});
	//}
	//static float RotY = 0.f;
	//RotY += 0.5f;
	//Quaternion q1 = Quaternion::AngleAxis(-90.f,   float3(1.f, 0.f, 0.f));	// 最初はx軸で-90度のクォータニオンの作成
	//Quaternion q2 = Quaternion::AngleAxis( 25.f,   float3(0.f, 0.f, 1.f));	// q1の状態からz軸に-25回転
	//Quaternion q3 = Quaternion::AngleAxis(-RotY,   float3(0.f, 1.f, 0.f));	// q2の状態からy軸に回転
	//m_transform->m_rotation = q1 * q2 * q3;
	//m_transform->m_rotation = Quaternion::Euler(float3(-90.f, -90.f, -90.f));
	PrintDebugProc("abcdefgHIGKLMNoPqRsTuVwXyZ123456789\n");
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