// 自機クラス [Player.cpp]
#include "Player.h"
#include "GameObject3D.h"
#include "input.h"
#include "debugproc.h"
#include "Missile.h"
#include "Scene.h"
#include "Collision.h"
#include "Sky.h"
#include "System.h"

// マクロ定義
#define SPEED			15.0f	// 速さ
static const float VAL_ANGLE_Z = 2.f;
static const float MAX_ANGLE_Z = 45.f;


// コンストラクタ
CPlayer::CPlayer() : m_roll(0.f), m_vMove(float3()), m_target(nullptr) {

}

// デストラクタ
CPlayer::~CPlayer() {

}


void CPlayer::Awake() {

}

// 初期化
void CPlayer::Start() {
	m_target = nullptr;
}

// 終了処理
void CPlayer::Uninit() {

}

// 更新
void CPlayer::Update()
{
	//bool move = true;

	// 移動
	//if (Input::isPress('A')) {
	//	if (m_transform->m_position.x > -MAX_MOVE_WIDTH) {
	//		m_vMove.x = -SPEED;
	//	}
	//	else {
	//		move = false;
	//	}
	//	if (m_roll < MAX_ANGLE_Z)
	//		m_roll += VAL_ANGLE_Z;
	//	if (m_roll < 0) {
	//		m_roll += VAL_ANGLE_Z;
	//	}
	//}
	//else if (Input::isPress('D')) {
	//	if (m_transform->m_position.x < MAX_MOVE_WIDTH) {
	//		m_vMove.x = SPEED;
	//	}
	//	else {
	//		move = false;
	//	}
	//	if (m_roll > -MAX_ANGLE_Z)
	//		m_roll -= VAL_ANGLE_Z;
	//	if (m_roll > 0) {
	//		m_roll -= VAL_ANGLE_Z;
	//	}
	//}
	//else if (Input::isPress('W')) {
	//	m_vMove.z = SPEED;
	//}
	//else if (Input::isPress('S')) {
	//	m_vMove.z = -SPEED;
	//}
	//// 上昇、下降
	//if (Input::isPress(VK_UP)) {
	//	m_vMove.y = SPEED * 0.5f;
	//}
	//else if (Input::isPress(VK_DOWN)) {
	//	m_vMove.y = -SPEED * 0.5f;
	//}
	//// 移動停止
	//if ((!Input::isPress('A') && !Input::isPress('D')) || !move) {
	//	if (m_vMove.x > 0.f) {
	//		m_vMove.x--;
	//	}
	//	else if (m_vMove.x < 0.f) {
	//		m_vMove.x++;
	//	}
	//	if (move) {
	//		if (m_roll > 0.f) {
	//			m_roll -= VAL_ANGLE_Z * 0.5f;
	//		}
	//		else if (m_roll < 0.f) {
	//			m_roll += VAL_ANGLE_Z * 0.5f;
	//		}
	//	}
	//}
	//if (!Input::isPress('W') && !Input::isPress('S')) {
	//	m_vMove.z = 0.f;
	//}
	//if (!Input::isPress(VK_UP) && !Input::isPress(VK_DOWN)) {
	//	m_vMove.y = 0;
	//}


	//m_transform->m_position.x += m_vMove.x;
	//m_transform->m_position.y += m_vMove.y;
	//m_transform->m_position.z += m_vMove.z;

	// モデル姿勢に依存しない平行移動
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_transform->m_rotate.x),
		XMConvertToRadians(m_transform->m_rotate.y),
		XMConvertToRadians(0.f)));
	float3 forward	= float3(mtx._31, mtx._32, mtx._33);
	float3 right	= float3(mtx._11, mtx._12, mtx._13);
	float3 up		= float3(mtx._21, mtx._22, mtx._23);

	if (Input::isPress('W')) {
		m_transform->m_position += forward * SPEED;
	}
	if (Input::isPress('S')) {
		m_transform->m_position -= forward * SPEED;
	}
	if (Input::isPress('D')) {
		m_transform->m_position += right * SPEED;
		if (m_roll > -MAX_ANGLE_Z)
			m_roll -= VAL_ANGLE_Z;
		if (m_roll > 0) {
			m_roll -= VAL_ANGLE_Z;
		}
	}
	if (Input::isPress('A')) {
		m_transform->m_position -= right * SPEED;
		if (m_roll < MAX_ANGLE_Z)
			m_roll += VAL_ANGLE_Z;
		if (m_roll < 0) {
			m_roll += VAL_ANGLE_Z;
		}
	}
	if (Input::isPress(VK_UP)) {
		m_transform->m_position += up * SPEED;
	}
	if (Input::isPress(VK_DOWN)) {
		m_transform->m_position -= up * SPEED;
	}

	// 移動停止
	if ((!Input::isPress('A') && !Input::isPress('D'))) {
		if (m_roll > 0.f) {
			m_roll -= VAL_ANGLE_Z * 0.5f;
		}
		else if (m_roll < 0.f) {
			m_roll += VAL_ANGLE_Z * 0.5f;
		}
	}

	m_transform->m_rotate.z = m_roll;

	// ターゲットロックオン
	if (Input::isTrigger('T')) {
		if (m_target == nullptr) {
			GameObject* obj = GameObject::FindGameObjectWithTag("Enemy");
			if (obj != nullptr) {
				m_target = obj;
			}
		}
		else if (m_target != nullptr) {
			m_target = nullptr;
		}
	}
	if (m_target != nullptr) {
		m_transform->LookAt(m_target->m_transform);
	}
	else {
		m_transform->m_rotate = float3();
	}


	// ホーミングミサイル発射
	if (Input::isTrigger(VK_SPACE)) {
		GameObject* obj = new GameObject3D(E_MODEL_MISSILE, "Missile");
		obj->AddComponent<Bullet>();
		obj->AddComponent<Collision>();
		obj->GetComponent<Collision>()->Init(E_MODEL_MISSILE);
		GameObject::Instance(obj, m_transform->m_position);
		obj->GetComponent<Collision>()->Update();
		obj->GetComponent<Collision>()->LastUpdate();
		obj->GetComponent<Bullet>()->Fire(*m_transform);
	}

	GameObject* sky = GameObject::Find("Sky");
	if (sky != nullptr) {
		sky->GetComponent<CSky>()->SetPos(m_transform->m_position);
	}

	PrintDebugProc("roll = %.2f\n", m_roll);
}



// EOF