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


void Player::Start() {
	m_gameObject->AddComponent<Collision>();

	m_roll		= 0.f;
	m_vMove		= float3();
	m_target	= nullptr;
	m_transform->m_position = float3(0.f, 0.f, 0.f);
}


void Player::Update() {
	
	this->Operation();
}



void Player::Operation() {

	// ターゲットロックオン
	if (Input::isTrigger('T')) {
		// メモ
		// 複数敵が居たら近い順(もしくはソート基準なし)で
		// 順番に敵をロックオンする
		// 今のままだとリストの1番目と2番目を交互に見続けるだけ
		std::list<GameObject*> objlist = GameObject::FindGameObjectsWithTag("Enemy");
		for (auto obj : objlist) {
			if (obj == nullptr) continue;	// 多分nullはないと思うけど一応
			if (m_target == nullptr) {		// そもそもロックオンしてない
				m_target = obj;
				break;
			}
			if (m_target != obj) {		// すでにロックオン状態なので、違う敵をロックオン
				m_target = obj;
				break;
			}
		}
	}
	if (m_target != nullptr) {
		m_transform->LookAt(m_target->m_transform);
	}
	// ターゲットロックオフ
	if (Input::isTrigger('O')) {
		if (m_target != nullptr) {
			m_target = nullptr;
			m_transform->m_rotate = Quaternion::identity;
		}
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
	//m_transform->m_rotate = Quaternion::AngleAxis(m_roll, m_transform->m_forward);
	m_transform->m_rotate.z = XMConvertToRadians(m_roll);


	// ホーミングミサイル発射
	if (Input::isTrigger(VK_SPACE)) {
		GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet");
		Instantiate(obj, m_transform->m_position, m_transform->m_rotate);
		obj->AddComponent<Bullet>();
	}
}


void Player::OnCollision(GameObject* obj) {
	if (obj->GetTag() == "Enemy") {
		obj->GetComponent<Collision>()->SetHit();
	}
}


// EOF