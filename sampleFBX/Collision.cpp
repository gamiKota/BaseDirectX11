/**
 * @file Collision.cpp
 */


/**
 * @include
 */
#include "Collision.h"
#include "Shader.h"
#include "Camera.h"
#include "D3DClass.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectMesh.h"
#include "Light.h"
#include "imgui.h"
#include "Geometory.h"
#include <map>
#include "System.h"


using namespace DirectX;



// 構造体定義
//----- 頂点座標
struct VERTEX {
	XMFLOAT3	position;
	XMFLOAT3	normal;
};


// コンストラクタ
Collision::Collision() : m_bHit(false) {
	m_selfTag.clear();
}

// デストラクタ
Collision::~Collision() {
}

void Collision::Awake() {
	GameObject3D* obj = dynamic_cast<GameObject3D*>(m_gameObject);
	if (obj != nullptr) {	// 3Dオブジェクトのコリジョン
		m_vScale = float3(100.f, 100.f, 100.f);	// ボックスの大きさ(基準値)
	}
}

// 初期化
void Collision::Start() {
	
}

// 終了処理
void Collision::Uninit()
{
	// タグの解放
	m_selfTag.clear();
}

void Collision::Update() {
}

// 更新
void Collision::LastUpdate() {

}

// 描画
void Collision::DebugDraw() {
#if _DEBUG

	//if (true) { return; }

	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 背面カリング(裏を描かない)
	D3DClass::GetInstance().SetZWrite(true);

	// シェーダ設定
	ShaderManager* shader = &ShaderManager::GetInstance();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	shader->BindVS(E_VS::VS_NORMAL);
	shader->BindGS(E_GS::GS_LINE);
	shader->BindPS(E_PS::PS_COLOR);
	pDeviceContext->DSSetShader(NULL, NULL, 0);
	pDeviceContext->HSSetShader(NULL, NULL, 0);
	pDeviceContext->CSSetShader(NULL, NULL, 0);

	shader->SetTexturePS(NULL);

	SHADER_WORLD buf;

	buf.mWorld = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&GetWorld()));
	shader->UpdateBuffer("MainWorld", &buf);

	DrawCube();

#endif
}


bool Collision::AABB(Collision obj1, Collision obj2) {

	bool hit = false;

	// 衝突判定(AABB)
	XMFLOAT4X4 world1 = obj1.GetWorld();
	XMFLOAT4X4 world2 = obj2.GetWorld();
	float& Ax = world1._41;
	float& Ay = world1._42;
	float& Az = world1._43;
	float& Aw = obj1.m_vScale.x;
	float& Ah = obj1.m_vScale.y;
	float& Ad = obj1.m_vScale.z;
	
	float& Bx = world2._41;
	float& By = world2._42;
	float& Bz = world2._43;
	float& Bw = obj2.m_vScale.x;
	float& Bh = obj2.m_vScale.y;
	float& Bd = obj2.m_vScale.z;
	
	hit = Ax - Aw <= Bx + Bw &&
		Bx - Bw <= Ax + Aw &&
		Ay - Ah <= By + Bh &&
		By - Bh <= Ay + Ah &&
		Az - Ad <= Bz + Bd &&
		Bz - Bd <= Az + Ad;

	return hit;
}


bool Collision::OBB(Collision obj1, Collision obj2) {

	// セルフタグ判定
	for (auto tag1 = obj1.m_selfTag.begin(); tag1 != obj1.m_selfTag.end(); tag1++) {
		for (auto tag2 = obj2.m_selfTag.begin(); tag2 != obj2.m_selfTag.end(); tag2++) {
			std::string temp1 = *tag1;
			std::string temp2 = *tag2;
			if (temp1 == temp2) {
				return false;
			}
		}
	}

	XMFLOAT4X4 world1 = obj1.m_gameObject->m_transform->GetMatrix();
	XMFLOAT4X4 world2 = obj2.m_gameObject->m_transform->GetMatrix();

	// ワールド空間上のOBB中心座標を求める
	XMFLOAT3 vPos1, vPos2;
	XMStoreFloat3(&vPos1, XMVector3TransformCoord(
		XMLoadFloat3(&obj1.m_vCenter), XMLoadFloat4x4(&world1)));
	XMStoreFloat3(&vPos2, XMVector3TransformCoord(
		XMLoadFloat3(&obj2.m_vCenter), XMLoadFloat4x4(&world2)));
	// 中心座標間のベクトルを求める
	XMVECTOR vD = XMVectorSet(vPos2.x - vPos1.x,
		vPos2.y - vPos1.y,
		vPos2.z - vPos1.z, 0.0f);
	// モデル座標軸を求める
	XMFLOAT3 v[6];
	v[0] = XMFLOAT3(world1._11, world1._12, world1._13);
	v[1] = XMFLOAT3(world1._21, world1._22, world1._23);
	v[2] = XMFLOAT3(world1._31, world1._32, world1._33);
	v[3] = XMFLOAT3(world2._11, world2._12, world2._13);
	v[4] = XMFLOAT3(world2._21, world2._22, world2._23);
	v[5] = XMFLOAT3(world2._31, world2._32, world2._33);
	XMVECTOR vN[6];
	for (int i = 0; i < 6; ++i)
		vN[i] = XMLoadFloat3(&v[i]);
	// OBBの大きさ(半分)の長さを掛けたベクトルを求める
	XMVECTOR vL[6];
	vL[0] = XMVectorSet(v[0].x * obj1.m_vScale.x, v[0].y * obj1.m_vScale.x, v[0].z * obj1.m_vScale.x, 0.0f);
	vL[1] = XMVectorSet(v[1].x * obj1.m_vScale.y, v[1].y * obj1.m_vScale.y, v[1].z * obj1.m_vScale.y, 0.0f);
	vL[2] = XMVectorSet(v[2].x * obj1.m_vScale.z, v[2].y * obj1.m_vScale.z, v[2].z * obj1.m_vScale.z, 0.0f);
	vL[3] = XMVectorSet(v[3].x * obj2.m_vScale.x, v[3].y * obj2.m_vScale.x, v[3].z * obj2.m_vScale.x, 0.0f);
	vL[4] = XMVectorSet(v[4].x * obj2.m_vScale.y, v[4].y * obj2.m_vScale.y, v[4].z * obj2.m_vScale.y, 0.0f);
	vL[5] = XMVectorSet(v[5].x * obj2.m_vScale.z, v[5].y * obj2.m_vScale.z, v[5].z * obj2.m_vScale.z, 0.0f);
	// 分離軸候補はモデル座標軸
	float fL, fD;
	for (int i = 0; i < 6; ++i) {
		XMVECTOR& vS = vN[i];	// 分離軸候補(正規化済のはず)
		// OBBの影(半分)の合計
		fL = 0.0f;
		for (int j = 0; j < 6; ++j) {
			XMStoreFloat(&fD, XMVector3Dot(vS, vL[j]));
			fL += fabsf(fD);
		}
		// 影の合計と中心間の距離の比較
		XMStoreFloat(&fD, XMVector3Dot(vS, vD));
		if (fL < fabsf(fD))
			return false;	// 当たっていない
	}
	// 分離軸候補は2辺の直交ベクトル
	XMVECTOR vS;	// 分離軸候補
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; ++j) {
			// 分離軸候補を求める
			vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));
			// OBBの影(半分)の合計
			fL = 0.0f;
			for (int k = 0; k < 6; ++k) {
				XMStoreFloat(&fD, XMVector3Dot(vS, vL[k]));
				fL += fabsf(fD);
			}
			// 影の合計と中心間の距離の比較
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			if (fL < fabsf(fD))
				return false;	// 当たっていない
		}
	}

	return true;	// 当たっている
}


DirectX::XMFLOAT4X4 Collision::GetWorld() {
	// 大きさ関係がややこしい
	// 最初のモデルのスケールを1と考えて、倍率で大きさを変更する
	XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
	XMFLOAT4X4 world;
	float3 scale = m_vScale * 2.f;
	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(scale.x, scale.y, scale.z));
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z));
	matrix *= XMLoadFloat4x4(&m_transform->GetMatrix());
	XMStoreFloat4x4(&world, matrix);
	return world;
}


void Collision::OnCollision(GameObject* obj) {

}


void Collision::SetImGuiVal() {
#if _DEBUG
	ImGui::InputFloat3("m_vCenter",		(float*)&m_vCenter);
	ImGui::InputFloat3("m_vBBox",		(float*)&m_vScale);
#endif
}

// EOF