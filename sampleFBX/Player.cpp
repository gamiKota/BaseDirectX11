// 自機クラス [Player.cpp]
#include "Player.h"
#include "GameObject.h"
#include "input.h"
#include "ModelManager.h"
#include "debugproc.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Missile.h"
#include "Scene.h"
#include "Collision.h"
#include "Sky.h"
#include "System.h"

// マクロ定義
#define SPEED			15.0f	// 速さ
static const float VAL_ANGLE_Z = 2.f;
static const float MAX_ANGLE_Z = 45.f;

// 外部グローバル変数 (model.cpp)
extern CFbxLight	g_light;		// 光源情報

// コンストラクタ
CPlayer::CPlayer() : m_roll(0.f) {
}

// デストラクタ
CPlayer::~CPlayer() {
}


void CPlayer::Awake() {
	m_transform = &m_gameObject->m_transform;
	m_vMove = XMFLOAT3();
}

// 初期化
void CPlayer::Start() {

}

// 終了処理
void CPlayer::Uninit() {
	// 境界ボックス終了処理
}

// 更新
void CPlayer::Update()
{
	bool move = true;

	// 移動
	if (Input::isPress('A')) {
		if (m_transform->_41 > -MAX_MOVE_WIDTH) {
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
		if (m_transform->_41 < MAX_MOVE_WIDTH) {
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

	// 座標をワールド行列に反映
	m_transform->_41 += m_vMove.x;
	m_transform->_42 += m_vMove.y; 
	m_transform->_43 += m_vMove.z + VAL_MOVE_PLAYER;	// 常に前進

	// モデルの更新
	ModelManager::GetInstance().Update(E_MODEL_PLAYER);
	
	// カメラ注視点を更新
	CCamera::Get()->SetLook({ m_transform->_41, m_transform->_42, m_transform->_43 });

	// ホーミングミサイル発射
	if (Input::isTrigger(VK_SPACE)) {
		GameObject* obj = new GameObject("Missile");
		obj->AddComponent<Bullet>();
		obj->AddComponent<Collision>();
		obj->GetComponent<Collision>()->Init(E_MODEL_MISSILE);
		GameObject::Instance(obj);
		obj->m_transform = m_gameObject->m_transform;
		obj->GetComponent<Bullet>()->Fire(obj->m_transform, XMFLOAT3());
	}

	GameObject* sky = GameObject::Find("Sky");
	if (sky != nullptr) {
		sky->GetComponent<CSky>()->SetPos({ m_transform->_41, m_transform->_42, m_transform->_43 });
	}

	PrintDebugProc("roll = %.2f\n", m_roll);
}

// 描画
void CPlayer::Draw()
{
	XMMATRIX matrix = XMLoadFloat4x4(m_transform);	// 行列(拡縮、回転、座標を手動で変更する場合)

	// 拡縮の変更
	//matrix = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), matrix);
	// 回転軸の変更
	matrix = XMMatrixMultiply(XMMatrixRotationZ(XMConvertToRadians(m_roll)), matrix);
	// 座標の変更
	//matrix = XMMatrixMultiply(XMMatrixTranslation(0.f, 175.f, 0.f), matrix);

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, matrix);

	ModelManager::GetInstance().Draw(E_MODEL_PLAYER, world);
}

//// 座標取得
//XMFLOAT3 CPlayer::GetPos()
//{
//	extern CPlayer g_player;	// main.cpp
//	XMFLOAT3 vPos(g_player.m_transform->_41,
//				  g_player.m_transform->_42,
//				  g_player.m_transform->_43);
//	return vPos;
//}
//
//// ワールド変換取得
//XMFLOAT4X4& CPlayer::GetWorld()
//{
//	extern CPlayer g_player;	// main.cpp
//	return g_player.m_transform;
//}
//
//// 境界ボックス情報取得
//void CPlayer::GetBBoxInfo(XMFLOAT3& vPos,
//						  XMFLOAT3& vBox)
//{
//	extern CPlayer g_player;	// main.cpp
//	vPos = g_player.m_box->m_vPos;	// 位置(ワールド空間)
//	vBox = g_player.m_box->m_vBBox;	// 大きさ(縦横奥行÷2)
//}
//
//// 衝突有無
//void CPlayer::SetHit(bool bHit)
//{
//	extern CPlayer g_player;	// main.cpp
//	g_player.m_bHit = bHit;
//}
