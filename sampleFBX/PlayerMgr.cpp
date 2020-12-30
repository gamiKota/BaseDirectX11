/**
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
	m_gameObject->AddComponent<PlayerState>();
	m_gameObject->AddComponent<PlayerOperation>();

	// 変数の初期化
	m_rigidbody->m_weight = E_WEIGHT::_2;
}


void PlayerMgr::Update() {
	
	Tween tween;

	float3 end = float3(20.f, 10.f, 30.f);
	if (Input::isTrigger('M')) {
		m_transform->DOMove(end, 2.f);
	}

	this->Operation();
}


void PlayerMgr::Uninit() {
}



void PlayerMgr::Operation() {
	//// モデル姿勢に依存しない平行移動
	//XMFLOAT4X4 mtx = XMFLOAT4X4();
	//XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(m_transform->m_rotate.x, m_transform->m_rotate.y, 0.f));
	//float3 right = float3(mtx._11, mtx._12, mtx._13);
	//float3 up = float3(mtx._21, mtx._22, mtx._23);
	//float3 forward = float3(mtx._31, mtx._32, mtx._33);
	//
	//if (Input::isPress('W')) {
	//	m_transform->m_position += forward * SPEED;
	//}
	//if (Input::isPress('S')) {
	//	m_transform->m_position -= forward * SPEED;
	//}
	//if (Input::isPress('D')) {
	//	m_transform->m_position += right * SPEED;
	//	if (m_roll >= -MAX_ANGLE_Z)
	//		m_roll -= VAL_ANGLE_Z;
	//}
	//if (Input::isPress('A')) {
	//	m_transform->m_position -= right * SPEED;
	//	if (m_roll <= MAX_ANGLE_Z)
	//		m_roll += VAL_ANGLE_Z;
	//}
	//if (Input::isPress(VK_UP)) {
	//	m_transform->m_position.y += SPEED;
	//}
	//if (Input::isPress(VK_DOWN)) {
	//	m_transform->m_position.y -= SPEED;
	//}
	//
	//// 移動停止
	//if ((!Input::isPress('A') && !Input::isPress('D'))) {
	//	if (m_roll > 0.f) {
	//		m_roll -= VAL_ANGLE_Z * 0.5f;
	//	}
	//	else if (m_roll < 0.f) {
	//		m_roll += VAL_ANGLE_Z * 0.5f;
	//	}
	//}
	////m_transform->m_rotate = Quaternion::AngleAxis(m_roll, m_transform->m_forward);
	//m_transform->m_rotate.z = XMConvertToRadians(m_roll);
}


void PlayerMgr::OnCollision(GameObject* obj) {

}


void PlayerMgr::SetImGuiVal() {
#if _DEBUG
	
#endif
}


// EOF