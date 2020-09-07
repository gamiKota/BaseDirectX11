// ホーミングミサイルクラス [Missile.cpp]
#include "Missile.h"
#include "GameObject.h"
#include "Camera.h"
#include "D3DClass.h"
#include "ModelManager.h"
#include "Collision.h"
#include "Score.h"
//#include "debugproc.h"
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

	// モデル座標軸取得
	XMFLOAT3 vX = { m_transform->GetMatrix()._11, m_transform->GetMatrix()._12, m_transform->GetMatrix()._13 };
	XMFLOAT3 vY = { m_transform->GetMatrix()._21, m_transform->GetMatrix()._22, m_transform->GetMatrix()._23 };
	XMFLOAT3 vZ = { m_transform->GetMatrix()._31, m_transform->GetMatrix()._32, m_transform->GetMatrix()._33 };

	// 座標の取得とクリア
	XMFLOAT3 vP = { m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z };
	m_transform->m_position = XMFLOAT3();
	//m_transform->GetMatrix()._41 = m_transform->GetMatrix()._42 = m_transform->GetMatrix()._43 = 0.0f;
	//PrintDebugProc("Missile={%.3f,%.3f,%.3f}\n",
	//	vP.x, vP.y, vP.z);

	//// ターゲットが有効か?
	//if (m_pEnemy) {
	//	// 敵機へのベクトルを求める
	//	XMFLOAT3 vPP = m_pEnemy->GetPos();
	//	XMVECTOR vTarget = XMVectorSet(vPP.x - vP.x,
	//		vPP.y - vP.y, vPP.z - vP.z, 0.0f);
	//	// 正規化
	//	vTarget = XMVector3Normalize(vTarget);

	//	// 進行方向との角度を求める
	//	float fDot;
	//	XMStoreFloat(&fDot, XMVector3Dot(
	//		XMLoadFloat3(&vZ), vTarget));
	//	// 計算誤差で範囲外になる場合に丸める
	//	if (fDot < -1.0f) fDot = -1.0f;
	//	if (fDot > 1.0f) fDot = 1.0f;
	//	float fAngle = XMConvertToDegrees(acosf(fDot));
	//	if (fAngle < -3.0f) fAngle = -3.0f;
	//	if (fAngle > 3.0f) fAngle = 3.0f;

	//	// 回転軸を求める
	//	XMVECTOR vAxis = XMVector3Cross(
	//		XMLoadFloat3(&vZ), vTarget);
	//	// 正規化
	//	vAxis = XMVector3Normalize(vAxis);

	//	// ワールド変換を回転
	//	XMMATRIX rotate = XMMatrixRotationAxis(
	//		vAxis, XMConvertToRadians(fAngle));
	//	XMMATRIX world = XMLoadFloat4x4(&GetMatrix());
	//	world = XMMatrixMultiply(world, rotate);
	//	XMStoreFloat4x4(&GetMatrix(), world);
	//	//PrintDebugProc("World={%.3f,%.3f,%.3f}\n",
	//	//	GetMatrix()._41, GetMatrix()._42, GetMatrix()._43);
	//}

	// 速度を座標に反映
	if (1) {
		vP.x += vZ.x * SPEED;
		vP.y += vZ.y * SPEED;
		vP.z += vZ.z * SPEED;
	}
	// 座標をワールド行列に反映
	m_transform->m_position.x = vP.x;
	m_transform->m_position.y = vP.y;
	m_transform->m_position.z = vP.z;

	// モデルの更新
	ModelManager::GetInstance().Update(E_MODEL_MISSILE);

	std::list<GameObject*> list = GameObject::FindGameObjectsWithTag("Enemy");
	for (auto obj : list) {
		if (m_gameObject->GetComponent<Collision>() != nullptr && obj->GetComponent<Collision>() != nullptr) {
			if (Collision::AABB(*m_gameObject->GetComponent<Collision>(), *obj->GetComponent<Collision>())) {
				Destroy(obj);
				Destroy(m_gameObject);
				if (GameObject::Find("Score") != nullptr) {
					GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
				}
			}
		}
	}
}

// 描画
void Bullet::Draw()
{
	if (m_nStat == 0) {
		return;
	}

	//XMMATRIX matrix = XMLoadFloat4x4(&m_transform->GetMatrix());	// 行列(拡縮、回転、座標を手動で変更する場合)

	//// 拡縮の変更
	////matrix = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), matrix);
	//// 回転軸の変更
	//matrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)), matrix);
	//// 座標の変更
	////matrix = XMMatrixMultiply(XMMatrixTranslation(0.f, 175.f, 0.f), matrix);

	//XMFLOAT4X4 world;
	//XMStoreFloat4x4(&world, matrix);

	ModelManager::GetInstance().Draw(E_MODEL_MISSILE, m_transform->GetMatrix());
}

//void Bullet::OnCollision(GameObject* obj) {
//	if (obj->GetTag() == "Enemy") {
//		GameObject::Destroy(obj);
//	}
//}

// 発射
void Bullet::Fire(XMFLOAT4X4 pWorld, XMFLOAT3 pOffset) {
	XMFLOAT3 vPos;
	XMStoreFloat3(&vPos,
		XMVector3TransformCoord(
			XMLoadFloat3(&pOffset),
			XMLoadFloat4x4(&pWorld)));

	m_transform->GetMatrix() = pWorld;
	m_transform->GetMatrix()._41 = vPos.x;
	m_transform->GetMatrix()._42 = vPos.y;
	m_transform->GetMatrix()._43 = vPos.z;

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