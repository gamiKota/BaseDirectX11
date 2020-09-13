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
 @ abridgement namespace
 */
using namespace DirectX;


/**
 * @constant
 */
static const float SPEED		= 15.0f;	// 速さ
static const float VAL_ANGLE_Z	= 2.f;
static const float MAX_ANGLE_Z	= 30.f;


void Player::Start() {
	m_gameObject->AddComponent<Collision>();
	m_gameObject->GetComponent<Collision>()->Init(E_MODEL_PLAYER);

	m_roll		= 0.f;
	m_vMove		= float3();
	m_target	= nullptr;
}


void Player::Update()
{
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
		GameObject::Instance(obj, m_transform->m_position, m_transform->m_rotate);
	}

	PrintDebugProc("roll = %.2f\n", m_roll);
}


// EOF