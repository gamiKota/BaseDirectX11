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
#include "PlayerBullet.h"
#include "Material.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "Status.h"
#include "PlayerHP.h"
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
	m_bullet = m_gameObject->AddComponent<PlayerBullet>();
	m_gameObject->AddComponent<PlayerHP>();

	// 変数の初期化
	m_collider->m_weight = E_WEIGHT::_3;

	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	m_status->SetHP(1000.f);

	GameObject3D* obj = dynamic_cast<GameObject3D*>(m_gameObject);
	obj->m_material->m_specular = DirectX::XMFLOAT4(50.f, 50.f, 50.f, 1.f);

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
	//PrintDebugProc("abcdefgHIGKLMNoPqRsTuVwXyZ123456789\n");
	//float3::Length()
}


void PlayerMgr::LastUpdate() {
	SHADER_PLAYER buf;
	XMFLOAT4 pos = XMFLOAT4(m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z, 0.f);
	buf.vPos = XMLoadFloat4(&pos);
	buf.vHpRate = m_status->m_HP / m_status->m_maxHP;
	ShaderManager::GetInstance().UpdateBuffer("Player", &buf);
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