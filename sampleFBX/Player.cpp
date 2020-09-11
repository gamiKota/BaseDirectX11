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
CPlayer::CPlayer() : m_roll(0.f), m_vMove(XMFLOAT3()), m_target(nullptr) {

}

// デストラクタ
CPlayer::~CPlayer() {

}


void CPlayer::Awake() {

}

// 初期化
void CPlayer::Start() {
	m_target = nullptr;
	GameObject* obj = GameObject::FindGameObjectWithTag("Enemy");
	if (obj != nullptr) {
		m_target = obj;
	}
}

// 終了処理
void CPlayer::Uninit() {

}

// 更新
void CPlayer::Update()
{
	bool move = true;

	// 移動
	if (Input::isPress('A')) {
		if (m_transform->m_position.x > -MAX_MOVE_WIDTH) {
			m_vMove.x = -SPEED;
		}
		else {
			move = false;
		}
		if (m_roll < MAX_ANGLE_Z)
			m_roll += VAL_ANGLE_Z;
		if (m_roll < 0) {
			m_roll += VAL_ANGLE_Z;
		}
	}
	else if (Input::isPress('D')) {
		if (m_transform->m_position.x < MAX_MOVE_WIDTH) {
			m_vMove.x = SPEED;
		}
		else {
			move = false;
		}
		if (m_roll > -MAX_ANGLE_Z)
			m_roll -= VAL_ANGLE_Z;
		if (m_roll > 0) {
			m_roll -= VAL_ANGLE_Z;
		}
	}
	if ((!Input::isPress('A') && !Input::isPress('D')) || !move) {
		if (m_vMove.x > 0.f) {
			m_vMove.x--;
		}
		else if (m_vMove.x < 0.f) {
			m_vMove.x++;
		}
		if (move) {
			if (m_roll > 0.f) {
				m_roll -= VAL_ANGLE_Z * 0.5f;
			}
			else if (m_roll < 0.f) {
				m_roll += VAL_ANGLE_Z * 0.5f;
			}
		}
	}
	if (Input::isPress(VK_LEFT)) {
		m_transform->m_rotate.y -= 1.f;
	}
	if (Input::isPress(VK_RIGHT)) {
		m_transform->m_rotate.y += 1.f;
	}

	// 座標をワールド行列に反映
	m_transform->m_position.x += m_vMove.x;
	m_transform->m_position.y += m_vMove.y; 
	m_transform->m_position.z += m_vMove.z + VAL_MOVE_PLAYER;	// 常に前進

	m_transform->m_rotate.z = m_roll;


	// ターゲットロックオン
	if (m_target != nullptr) {
		//float3 vec = float3(m_target->m_transform->m_position.x - )
		m_transform->m_rotate.y = XMConvertToDegrees(atan2f(
			m_target->m_transform->m_position.x - m_transform->m_position.x,
			m_target->m_transform->m_position.z - m_transform->m_position.z));
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