// 三人称カメラ [TPCamera.cpp]
#include "TPCamera.h"
#include "Player.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject3D.h"
#include "System.h"


// 定数
namespace {
	const XMFLOAT3 g_vEye = XMFLOAT3(0.0f, 150.0f, -500.0f);	// X-wing
	const XMFLOAT3 g_vLook = XMFLOAT3(0.0f, 120.0f, 0.0f);
	const XMFLOAT3 g_vUp(0.0f, 1.0f, 0.0f);
};

// 初期化
void TPCamera::Awake() {
	m_vEye = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
	m_fFOVY = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
}

void TPCamera::Start() {
	m_player = GameObject::Find("Player");
	if (m_player == nullptr) {
		MessageBoxA(
			System::GetInstance().GetWnd(),
			"error player nullptr\nload title scene", "TPCamera.cpp",
			MB_OK | MB_ICONERROR | MB_TOPMOST
		);
		SceneManager::GetInstance().LoadScene(E_SCENE::TITLE);
	}
	Update();
}

// 終了処理
void TPCamera::Uninit() {
	CCamera::Uninit();
}

// 更新
void TPCamera::Update()
{
	// メモ
	// 注視点はロックオンされているターゲット(ロックオンしてない時はプレイヤー)
	// 座標はターゲットとプレイヤーのベクトル上の少し後ろでプレイヤーのY軸から真横に移動
	// 視点と注視点を移動、上方ベクトルを回転
	XMMATRIX world = XMLoadFloat4x4(&m_player->m_transform->GetMatrix());
	// 座標
	XMStoreFloat3(&m_vEye, XMVector3TransformCoord(XMLoadFloat3(&g_vEye), world));
	// 注視点
	XMStoreFloat3(&m_vLook, XMVector3TransformCoord(XMLoadFloat3(&g_vLook), world));
	// 上方ベクトル
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&g_vUp), world));

	if (m_player->GetComponent<CPlayer>()->m_target != nullptr) {	// ターゲットロックオン状態
		float3 eye = m_player->m_transform->m_position;
		eye -= m_player->m_transform->m_forward * 400.f;
		m_vEye = XMFLOAT3(eye.x, eye.y + 100.f, eye.z);

		XMFLOAT3 look = XMFLOAT3(
			m_player->GetComponent<CPlayer>()->m_target->m_transform->m_position.x,
			m_player->GetComponent<CPlayer>()->m_target->m_transform->m_position.y,
			m_player->GetComponent<CPlayer>()->m_target->m_transform->m_position.z);
		m_vLook = look;

		m_vUp = XMFLOAT3(0.f, 1.f, 0.f);
	}

	//行列更新
	CCamera::Update();
}
