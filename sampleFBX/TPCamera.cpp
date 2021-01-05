/**
 * TPCamera.cpp
 */


/**
 * @include
 */
#include "TPCamera.h"
#include "PlayerMgr.h"
#include "PlayerState.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject3D.h"
#include "input.h"
#include "debugproc.h"
#include "System.h"


void TPCamera::Awake() {
}


void TPCamera::Start() {
	CCamera::Start();
	m_fFarZ = 25000.0f;
	m_player = GameObject::Find("Player")->GetComponent<PlayerMgr>();
}


void TPCamera::Uninit() {
	CCamera::Uninit();
}


void TPCamera::Update() {

	// 追従処理
	// プレイヤーの横移動でずらしたい
	// 処理順のせいなのかガクつく
	if (m_player != nullptr) {
		GameObject* target = m_player->m_state->GetTarget();
		// モデル姿勢に依存しない平行移動
		XMFLOAT4X4 mtx = XMFLOAT4X4();
		float3 rotate = Quaternion::RadianAngle(m_player->m_transform->m_rotation);
		XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, 0.f));
		float3 right = float3(mtx._11, mtx._12, mtx._13);
		float3 up = float3(mtx._21, mtx._22, mtx._23);
		float3 forward = float3(mtx._31, mtx._32, mtx._33);
		if (target != nullptr) {	// ターゲットオン
			//m_transform->LookAt(target->m_transform);
			m_transform->LookAt(target->m_transform, m_player->m_transform->m_up);
			//SetLook(target->m_transform);
		}
		else {	// ターゲットオフ
			m_transform->LookAt(m_player->m_transform);
			//SetLook(nullptr);
		}
		m_transform->m_position = m_player->m_transform->m_position;
		m_transform->m_position -= forward * 500.f;
		m_transform->m_position += up * 150.f;
		//CCamera::Get()->SetLook(m_player->m_transform);
	}

	CCamera::Update();
}


void TPCamera::LastUpdate() {
	// メモ
	// 注視点はロックオンされているターゲット(ロックオンしてない時はプレイヤー)
	// 座標はターゲットとプレイヤーのベクトル上の少し後ろでプレイヤーのY軸から真横に移動
	// 視点と注視点を移動、上方ベクトルを回転

	//行列更新
	CCamera::LastUpdate();
}

// EOF