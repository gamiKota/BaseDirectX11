/**
 * @file Player.cpp
 */


/**
 * @include
 */
#include "Player.h"
#include "GameObject3D.h"
#include "input.h"
#include "debugproc.h"
#include "Bullet.h"
#include "Collision.h"
#include "System.h"


/**
 * @abridgement namespace
 */
using namespace DirectX;


/**
 * @constant
 */
static const float SPEED		= 15.0f;	// 速さ
static const float VAL_ANGLE_Z	= 2.f;
static const float MAX_ANGLE_Z	= 30.f;

static const float MAX_TIME = 10.f * 60.f;

float g_time = 0.f;
Quaternion q1, q2;


void Player::Start() {
	m_gameObject->AddComponent<Collision>();

	m_roll		= 0.f;
	m_vMove		= float3();
	m_target	= nullptr;
	m_transform->m_position = float3(0.f, 0.f, 0.f);

	g_time = 0.f;
}


void Player::Update() {
	
	XMFLOAT3 f1;
	XMFLOAT4 result;
	f1.x = 0.f;
	f1.y = 0.f;
	f1.z = 1.f;

	XMStoreFloat4(&result, XMQuaternionRotationAxis(XMLoadFloat3(&f1), XMConvertToRadians(g_time)));

	m_transform->m_rotate.z += XMConvertToRadians(g_time);

	PrintDebugProc("axisAngle = %.2f, %.2f, %.2f, %.2f\n",
		m_transform->m_rotate.x, m_transform->m_rotate.y, m_transform->m_rotate.z, m_transform->m_rotate.w);

	g_time += 1.f;
	if (g_time >= 720.f) {
		g_time = 0.f;
	}

	



	//this->Operation();
}



void Player::Operation() {

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
			m_transform->m_rotate = Quaternion::identity;
		}
	}
	if (m_target != nullptr) {
		m_transform->LookAt(m_target->m_transform);
	}

	// モデル姿勢に依存しない平行移動
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(m_transform->m_rotate.x, m_transform->m_rotate.y, 0.f));
	float3 right = float3(mtx._11, mtx._12, mtx._13);
	float3 up = float3(mtx._21, mtx._22, mtx._23);
	float3 forward = float3(mtx._31, mtx._32, mtx._33);
	
	if (Input::isPress('W')) {
		m_transform->m_position += forward * SPEED;
	}
	if (Input::isPress('S')) {
		m_transform->m_position -= forward * SPEED;
	}
	if (Input::isPress('D')) {
		m_transform->m_position += right * SPEED;
		if (m_roll >= -MAX_ANGLE_Z)
			m_roll -= VAL_ANGLE_Z;
	}
	if (Input::isPress('A')) {
		m_transform->m_position -= right * SPEED;
		if (m_roll <= MAX_ANGLE_Z)
			m_roll += VAL_ANGLE_Z;
	}
	if (Input::isPress(VK_UP)) {
		m_transform->m_position.y += SPEED;
	}
	if (Input::isPress(VK_DOWN)) {
		m_transform->m_position.y -= SPEED;
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
	m_transform->m_rotate = Quaternion::AngleAxis(m_roll, m_transform->m_forward);


	// ホーミングミサイル発射
	if (Input::isTrigger(VK_SPACE)) {
		GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet");
		Instantiate(obj, m_transform->m_position, m_transform->m_rotate);
		obj->AddComponent<Bullet>();
	}
}


void Player::OnCollision(GameObject* obj) {
	PrintDebugProc("HIT!!!!!!!!!!");
}


// EOF