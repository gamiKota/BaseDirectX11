/**
 * @EnemyState.cpp
 */


/**
 * @include
 */
 // 自身の宣言先
#include "EnemyState.h"
// オブジェクトシステム
#include "Transform.h"
#include "GameObject.h"
#include "GameObject3D.h"
// コンポーネント
#include "Bullet.h"
// システム
#include "debugproc.h"
#include "System.h"


/**
 * @abridgement namespace
 */
using namespace DirectX;


/**
 * @constant
 */
static const float SPEED = 15.0f;	// 速さ
static const float VAL_ANGLE_Z = 2.f;
static const float MAX_ANGLE_Z = 30.f;


// ステートマシンの初期化
void EnemyState::Initialize() {
	// 状態の追加
	StateMachine::AddState(new EnemyState::Idol(this), true);
	StateMachine::AddState(new EnemyState::Move(this));
	StateMachine::AddState(new EnemyState::TargetOn(this));
	StateMachine::AddState(new EnemyState::TargetOff(this));
	StateMachine::AddState(new EnemyState::AttackBullet(this));
}

// 状態に依存しない共通処理
void EnemyState::Update() {
	StateMachine::Update();

	Quaternion q = m_transform->m_rotation;
	Quaternion qz = Quaternion::AngleAxis(m_rotate.z, m_transform->m_forward);
	Quaternion qy = Quaternion::AngleAxis(m_rotate.y, float3(0.f, 1.f, 0.f));
	m_transform->m_rotation = q * qy * qz;
}


/**************************************************************************************************
 * @state 静止
 *************************************************************************************************/
void EnemyState::Idol::Start() {
	m_main->SetStateActive(ENEMY_STATE::MOVE, false);
}

void EnemyState::Idol::Update() {
	if (m_main->m_rotate.z > 0.f) {
		m_main->m_rotate.z -= VAL_ANGLE_Z * 0.5f;
	}
	else if (m_main->m_rotate.z < 0.f) {
		m_main->m_rotate.z += VAL_ANGLE_Z * 0.5f;
	}
}

void EnemyState::Idol::OnDestoy() {
}


/**************************************************************************************************
 * @state 移動
 *************************************************************************************************/
void EnemyState::Move::Start() {
	m_main->SetStateActive(ENEMY_STATE::IDOL, false);
}

void EnemyState::Move::Update() {
	// モデル姿勢に依存しない平行移動
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	float3 rotate = Quaternion::RadianAngle(m_main->m_transform->m_rotation);

	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, 0.f));
	float3 right = float3(mtx._11, mtx._12, mtx._13);
	float3 up = float3(mtx._21, mtx._22, mtx._23);
	float3 forward = float3(mtx._31, mtx._32, mtx._33);

	// Z軸移動
	if (m_movement.z != 0.f) {
		m_main->m_transform->m_position += forward * (SPEED * m_movement.z);
	}
	// X軸移動
	if (m_movement.x != 0.f) {
		m_main->m_transform->m_position += right * (SPEED * m_movement.x);
		if (m_movement.x > 0.f) {
			if (m_main->m_rotate.z >= -MAX_ANGLE_Z)
				m_main->m_rotate.z -= VAL_ANGLE_Z;
		}
		else {
			if (m_main->m_rotate.z <= MAX_ANGLE_Z)
				m_main->m_rotate.z += VAL_ANGLE_Z;
		}
	}
	else {
		if (m_main->m_rotate.z > 0.f) {
			m_main->m_rotate.z -= VAL_ANGLE_Z * 0.5f;
		}
		else if (m_main->m_rotate.z < 0.f) {
			m_main->m_rotate.z += VAL_ANGLE_Z * 0.5f;
		}
	}
	// Y軸移動(ターゲットの方に向いてるのでy要素で直接移動処理)
	if (m_movement.y != 0.f) {
		m_main->m_transform->m_position.y += (SPEED * m_movement.y);
	}
}

void EnemyState::Move::OnDestoy() {
}


/**************************************************************************************************
 * @state ターゲットON
 *************************************************************************************************/
void EnemyState::TargetOn::Start() {
	// 変数の初期化
	m_target = nullptr;
	m_maxAngle = 1.f;
	m_main->SetStateActive(ENEMY_STATE::TARGET_OFF, false);
}

void EnemyState::TargetOn::Update() {
	if (m_target == nullptr) {
		m_main->SetStateActive(ENEMY_STATE::TARGET_OFF, true);
		return;
	}

	// ターゲット方向のベクトルを取得
	float3 v1 = m_target->m_transform->m_position - m_main->m_transform->m_position;
	// 回転情報の格納
	Quaternion q1 = m_main->m_transform->m_rotation;
	Quaternion q2 = Quaternion::LookRotation(v1);
	// 現在の回転情報と、ターゲット方向の回転情報を角度制限付きで補間する
	m_main->m_transform->m_rotation = Quaternion::RotateTowards(q1, q2, m_maxAngle);
}

void EnemyState::TargetOn::OnDestoy() {
	m_target = nullptr;
}


/**************************************************************************************************
 * @state ターゲットOFF(敵が何もターゲットしてない時ある？)
 *************************************************************************************************/
void EnemyState::TargetOff::Start() {
	m_main->SetStateActive(ENEMY_STATE::TARGET_ON, false);
}

void EnemyState::TargetOff::Update() {
	m_main->m_rotate.y = 0.f;
}

void EnemyState::TargetOff::OnDestoy() {
}


/**************************************************************************************************
 * @state 射撃
 *************************************************************************************************/
void EnemyState::AttackBullet::Start() {
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, m_main->m_transform->m_position + m_main->m_transform->m_forward * 200.f, m_main->m_transform->m_rotation);
	obj->AddComponent<Bullet>();
	// Start関数で撃ち終わったので状態終了
	m_main->SetStateActive(ENEMY_STATE::ATTACK_BULLET, false);
}

void EnemyState::AttackBullet::Update() {
}

void EnemyState::AttackBullet::OnDestoy() {
}


// EOF