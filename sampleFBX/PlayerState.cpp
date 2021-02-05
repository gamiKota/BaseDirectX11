/**
 * @file PlayerState.cpp
 */


/**
 * @include
 */
// 自身の宣言先
#include "PlayerState.h"
// オブジェクトシステム
#include "Transform.h"
#include "GameObject.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
// コンポーネント
#include "Status.h"
#include "Enemy.h"
#include "Bullet.h"
#include "HPGauge.h"
#include "Material.h"
// システム
#include "input.h"
#include "debugproc.h"
#include "System.h"


/**
 * @abridgement namespace
 */
using namespace DirectX;


/**
 * @constant
 */
static const float SPEED = 25.0f;	// 速さ
static const float VAL_ANGLE_Z = 2.f;
static const float MAX_ANGLE_Z = 30.f;


// ステートマシンの初期化
void PlayerState::Initialize() {
	// 変数の初期化
	m_rotate = float3();
	m_targetLogo = new GameObjectUI(E_LAYER::UI, E_TEXTURE_ROCK_ICON_TARGET, "PlayerTargetLogo", "PlayerTargetLogo");
	GameObject::Instantiate(m_targetLogo, float3(115.f, -55.f, 0.f), Quaternion::identity, float3(130.f, 40.f, 0.f));
	// HPゲージ
	m_HPGauge = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_TREE, "HPGauge", "HPGauge");
	m_HPGauge->AddComponent<HPGauge>();
	m_HPGauge->m_ps = E_PS::PS_HPGAUGE;
	GameObject::Instantiate(m_HPGauge);

	// 状態の追加
	StateMachine::AddState(new PlayerState::Idol(this), true);
	StateMachine::AddState(new PlayerState::Move(this));
	StateMachine::AddState(new PlayerState::TargetOn(this));
	StateMachine::AddState(new PlayerState::TargetOff(this), true);
	StateMachine::AddState(new PlayerState::AttackBullet(this));
}

// 状態に依存しない共通処理
void PlayerState::Update() {
	StateMachine::Update();

	Quaternion q = m_transform->m_rotation;
	Quaternion q1 = Quaternion::AngleAxis(m_rotate.z, m_transform->m_forward);
	m_transform->m_rotation = q * q1;
}


/**************************************************************************************************
 * @state 静止
 *************************************************************************************************/
void PlayerState::Idol::Start() {
	main->SetStateActive(PLAYER_STATE::MOVE, false);
}

void PlayerState::Idol::Update() {
	if (main->m_rotate.z > 0.f) {
		main->m_rotate.z -= VAL_ANGLE_Z * 0.5f;
	}
	else if (main->m_rotate.z < 0.f) {
		main->m_rotate.z += VAL_ANGLE_Z * 0.5f;
	}
}

void PlayerState::Idol::OnDestoy() {
}


/**************************************************************************************************
 * @state 移動
 *************************************************************************************************/
void PlayerState::Move::Start() {
	main->SetStateActive(PLAYER_STATE::IDOL, false);
}

void PlayerState::Move::Update() {
	// モデル姿勢に依存しない平行移動
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	float3 rotate = Quaternion::RadianAngle(main->m_transform->m_rotation);

	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, 0.f));
	float3 right = float3(mtx._11, mtx._12, mtx._13);
	float3 up = float3(mtx._21, mtx._22, mtx._23);
	float3 forward = float3(mtx._31, mtx._32, mtx._33);

	// Z軸移動
	if (main->m_movement.z != 0.f) {
		main->m_transform->m_position += forward * (SPEED * main->m_movement.z);
	}
	// X軸移動
	if (main->m_movement.x != 0.f) {
		main->m_transform->m_position += right * (SPEED * main->m_movement.x);
		if (main->m_movement.x > 0.f) {
			if (main->m_rotate.z >= -MAX_ANGLE_Z)
				main->m_rotate.z -= VAL_ANGLE_Z;
		}
		else {
			if (main->m_rotate.z <= MAX_ANGLE_Z)
				main->m_rotate.z += VAL_ANGLE_Z;
		}
	}
	else {
		if (main->m_rotate.z > 0.f) {
			main->m_rotate.z -= VAL_ANGLE_Z * 0.5f;
		}
		else if (main->m_rotate.z < 0.f) {
			main->m_rotate.z += VAL_ANGLE_Z * 0.5f;
		}
	}
	// Y軸移動(ターゲットの方に向いてるのでy要素で直接移動処理)
	if (main->m_movement.y != 0.f) {
		main->m_transform->m_position.y += ((SPEED * 0.5f) * main->m_movement.y);
		//main->m_transform->m_position += up * (SPEED * main->m_movement.y);
	}
}

void PlayerState::Move::OnDestoy() {
}


/**************************************************************************************************
 * @state ターゲットON
 *************************************************************************************************/
void PlayerState::TargetOn::Start() {
	// 変数の初期化
	main->m_target = nullptr;
	main->m_targetLogo->m_material->m_diffuse.w = 1.f;
	main->m_HPGauge->m_material->m_diffuse.w = 1.f;
	m_cnt = 0;
	// ターゲット処理
	main->SetStateActive(PLAYER_STATE::TARGET_OFF, false);
	std::list<GameObject*> objlist = GameObject::FindGameObjectsWithTag("Enemy");
	for (auto obj : objlist) {
		if (main->m_target != obj) {		// すでにロックオン状態なので、違う敵をロックオン
			main->m_target = obj;
			break;
		}
	}
}
void PlayerState::TargetOn::Start(bool active) {
	// まだ優先順位によるロックオンになってない
	if (active) {
		m_cnt++;
		std::list<GameObject*> objlist = GameObject::FindGameObjectsWithTag("Enemy");
		if ((unsigned)m_cnt >= objlist.size()) {
			m_cnt = 0;
		}
		auto l_front = objlist.begin();
		std::advance(l_front, m_cnt);
		main->m_target = *l_front;
	}
}

void PlayerState::TargetOn::Update() {
	if (main->m_target == nullptr) {
		main->SetStateActive(PLAYER_STATE::TARGET_OFF, true);
		return;
	}
	main->m_transform->LookAt(main->m_target->m_transform);

	Character* enemy = dynamic_cast<Character*>(main->m_target->GetComponent<Character>());
	HPGauge* hp = main->m_HPGauge->GetComponent<HPGauge>();
	hp->m_HP = enemy->m_status->m_HP;
	hp->m_maxHP = enemy->m_status->m_maxHP;
}

void PlayerState::TargetOn::OnDestoy() {
	main->m_target = nullptr;
}


/**************************************************************************************************
 * @state ターゲットOFF
 *************************************************************************************************/
void PlayerState::TargetOff::Start() {
	main->m_targetLogo->m_material->m_diffuse.w = 0.f;
	main->m_HPGauge->m_material->m_diffuse.w = 0.f;
	main->SetStateActive(PLAYER_STATE::TARGET_ON, false);
}

void PlayerState::TargetOff::Update() {
	// Y軸に回転して欲しくない
	main->m_transform->m_rotation = Quaternion::AngleAxis(0.f, float3(0.f, 1.f, 0.f));
}

void PlayerState::TargetOff::OnDestoy() {
}


/**************************************************************************************************
 * @state 射撃
 *************************************************************************************************/
void PlayerState::AttackBullet::Start() {
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, main->m_transform->m_position + main->m_transform->m_forward * 200.f, main->m_transform->m_rotation);
	obj->AddComponent<Bullet>();
	// Start関数で撃ち終わったので状態終了
	main->SetStateActive(PLAYER_STATE::ATTACK_BULLET, false);
}

void PlayerState::AttackBullet::Update() {
}

void PlayerState::AttackBullet::OnDestoy() {
}



// EOF