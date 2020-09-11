// ホーミングミサイルクラス [Missile.cpp]
#include "Missile.h"
#include "GameObject.h"
#include "Collision.h"
#include "Score.h"
#include "Player.h"
#include "System.h"

// マクロ定義
#define SPEED			50.0f		// 速さ


// コンストラクタ
Bullet::Bullet() : m_nStat(0), m_nLife(0) {
}

// デストラクタ
Bullet::~Bullet() {

}


void Bullet::Awake() {

}

// 初期化
void Bullet::Start() {
	m_nLife = 0;
	m_nStat = 0;
}

// 終了処理
void Bullet::Uninit() {
}

// 更新
void Bullet::Update() {
	// 未使用?
	if (m_nStat == 0) {
		return;
	}
	// 追尾中 (m_nStat == 1)
	--m_nLife;
	if (m_nLife <= 0) {
		// 爆発開始
		m_nLife = 3 * 60;
		m_nStat = 0;
		GameObject::Destroy(m_gameObject);
		return;
	}
	// 前に進む
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	if (obj->GetTag() == "Enemy") {
		GameObject::Destroy(obj);
		Destroy(m_gameObject);
		if (GameObject::Find("Score") != nullptr) {
			GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
		}
		if (GameObject::Find("Player") != nullptr) {
			if (obj == GameObject::Find("Player")->GetComponent<CPlayer>()->m_target)
				GameObject::Find("Player")->GetComponent<CPlayer>()->m_target = nullptr;
		}
	}
}

// 発射
void Bullet::Fire(Transform transform) {

	m_transform->m_position = transform.m_position;
	m_transform->m_rotate = transform.m_rotate;
	m_transform->m_scale = { 1.f, 1.f, 1.f };


	m_nLife = 1 * 60;	// 5秒
	m_nStat = 1;		// 追尾中


	// ターゲットが有効か?
	//if (m_pEnemy) {
		//// モデル座標軸取得
		//XMFLOAT3 vX = { m_transform->_11, m_transform->_12, m_transform->_13 };
		//XMFLOAT3 vY = { m_transform->_21, m_transform->_22, m_transform->_23 };
		//XMFLOAT3 vZ = { m_transform->_31, m_transform->_32, m_transform->_33 };
		//// 敵機へのベクトルを求める
		//XMFLOAT3 vP = { m_transform->_41, m_transform->_42, m_transform->_43 };
		//XMFLOAT3 vPP = XMFLOAT3();// = m_pEnemy->GetPos();
		//XMVECTOR vTarget = XMVectorSet(vPP.x - vP.x,
		//	vPP.y - vP.y, vPP.z - vP.z, 0.0f);
		////正規化
		//vTarget = XMVector3Normalize(vTarget);

		//// 進行方向との角度を求める
		//float fDot;
		//XMStoreFloat(&fDot, XMVector3Dot(
		//	XMLoadFloat3(&vZ), vTarget));
		//// 計算誤差で範囲外になる場合に丸める
		//if (fDot < -1.0f) fDot = -1.0f;
		//if (fDot > 1.0f) fDot = 1.0f;
		//float fAngle = XMConvertToDegrees(acosf(fDot));
		//if (fAngle < -3.0f) fAngle = -3.0f;
		//if (fAngle > 3.0f) fAngle = 3.0f;

		//// 回転軸を求める
		//XMVECTOR vAxis = XMVector3Cross(
		//	XMLoadFloat3(&vZ), vTarget);
		//// 正規化
		//vAxis = XMVector3Normalize(vAxis);

		// ワールド変換を回転
		//XMMATRIX rotate = XMMatrixRotationAxis(
		//	vAxis, XMConvertToRadians(fAngle));
		//XMMATRIX world = XMLoadFloat4x4(m_transform);
		//world = XMMatrixMultiply(world, rotate);
		//XMStoreFloat4x4(m_transform, world);
		//PrintDebugProc("World={%.3f,%.3f,%.3f}\n",
		//	m_transform->_41, m_transform->_42, m_transform->_43);
	//}
}

// EOF