/**
 * @file Collision.cpp
 */


/**
 * @include
 */
#include "Collision.h"
#include "Shader.h"
#include "Camera.h"
#include "FbxModel.h"
#include "D3DClass.h"
#include "GameObject3D.h"
#include "GameObjectUI.h"
#include "GameObjectMesh.h"
#include "Light.h"
#include "imgui.h"
#include "System.h"



using namespace DirectX;



// 構造体定義
//----- 頂点座標
struct VERTEX {
	XMFLOAT3	position;
	XMFLOAT3	normal;
};


// コンストラクタ
Collision::Collision() : m_color(1.0f, 1.0f, 1.0f, 0.5f), m_bHit(false), m_isInit(false) {
	m_selfTag.clear();
}

// デストラクタ
Collision::~Collision() {
}

void Collision::Awake() {
	
}

// 初期化
void Collision::Start() {
	
}

// 終了処理
void Collision::Uninit()
{
	// 頂点バッファ解放
	SAFE_RELEASE(m_pVertexBuffer);
	// インデックスバッファ解放
	SAFE_RELEASE(m_pIndexBuffer);
	// タグの解放
	m_selfTag.clear();
}

void Collision::Update() {
	m_bHit = false;
}

// 更新
void Collision::LastUpdate() {
	m_bHit = false;

	if (!m_isInit) {
		GameObject3D* obj = dynamic_cast<GameObject3D*>(GameObject::Find(m_gameObject->m_name));
		if (obj != nullptr) {
			Init(obj->m_model);
			m_isInit = true;
		}
	}
}

// 描画
void Collision::DebugDraw() {
#if _DEBUG

	if (!m_isInit)	return;

	//if (true) { return; }

	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 背面カリング(裏を描かない)
	D3DClass::GetInstance().SetZWrite(true);

	// 境界ボックスの色
	if (m_bHit) {
		m_color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);
	}
	else {
		m_color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);
	}

	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // プリミティブ形状をセット

	ShaderManager::GetInstance().BindPS(E_PS::E_PS_NORMAL);
	ShaderManager::GetInstance().BindVS(E_VS::E_VS_NORMAL);
	pDeviceContext->GSSetShader(NULL, NULL, 0);
	pDeviceContext->DSSetShader(NULL, NULL, 0);
	pDeviceContext->HSSetShader(NULL, NULL, 0);
	pDeviceContext->CSSetShader(NULL, NULL, 0);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファ設定
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// 定数バッファ設定
	
	SHADER_WORLD world;
	world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&GetWorld()));
	ShaderManager::GetInstance().UpdateBuffer("MainWorld", &world);

	SHADER_MATERIAL material;
	material.vDiffuse = XMLoadFloat4(&m_color);
	material.vAmbient = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	material.vSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	material.vEmissive = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ShaderManager::GetInstance().UpdateBuffer("Material", &material);

	// 描画
	pDeviceContext->DrawIndexed(36, 0, 0);

	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);	// 前面カリング(表を描かない)
	D3DClass::GetInstance().SetZWrite(true);

#endif
}


void Collision::Init(E_MODEL model) {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	if (model >= E_MODEL_NONE && model < E_MODEL_MAX) {
		// 境界ボックス初期化
		//m_model = model;
		//XMFLOAT3 temp = ModelManager::GetInstance().Get(model)->GetCenter();
		//m_vCenter = float3(temp.x, temp.y, temp.z);
		//temp = ModelManager::GetInstance().Get(model)->GetBBox();
		//m_vBBox = float3(temp.x, temp.y, temp.z);

		m_vCenter = float3();
		m_vBBox = float3();
	}

	// インデックス バッファ生成
	int index[36] = { 0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9, 10, 10, 9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23,
	};
	CD3D11_BUFFER_DESC ibd(sizeof(index), D3D11_BIND_INDEX_BUFFER);
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = index;
	hr = pDevice->CreateBuffer(&ibd, &initData, &m_pIndexBuffer);

	// 頂点バッファ生成
	static XMFLOAT3 pos[24] = {
		{-1,  1, -1}, { 1,  1, -1}, {-1, -1, -1}, { 1, -1, -1},
		{ 1,  1, -1}, { 1,  1,  1}, { 1, -1, -1}, { 1, -1,  1},
		{ 1,  1,  1}, {-1,  1,  1}, { 1, -1,  1}, {-1, -1,  1},
		{-1,  1,  1}, {-1,  1, -1}, {-1, -1,  1}, {-1, -1, -1},
		{-1,  1,  1}, { 1,  1,  1}, {-1,  1, -1}, { 1,  1, -1},
		{-1, -1, -1}, { 1, -1, -1}, {-1, -1,  1}, { 1, -1,  1},
	};
	static XMFLOAT3 dir[6] = {
		{ 0,  0, -1},
		{ 1,  0,  0},
		{ 0,  0,  1},
		{-1,  0,  0},
		{ 0,  1,  0},
		{ 0, -1,  0},
	};
	VERTEX vertex[24];
	for (int i = 0; i < 24; ++i) {
		vertex[i].position.x = m_vBBox.x * pos[i].x;
		vertex[i].position.y = m_vBBox.y * pos[i].y;
		vertex[i].position.z = m_vBBox.z * pos[i].z;
		vertex[i].normal = dir[i / 4];
	}
	// 頂点バッファ生成
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(vertex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertex;
	hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "box init", "error", MB_OK | MB_ICONWARNING | MB_TOPMOST);
	}
}


bool Collision::AABB(Collision obj1, Collision obj2) {

	bool hit = false;

	// 衝突判定(AABB)
	XMFLOAT4X4 world1 = obj1.GetWorld();
	XMFLOAT4X4 world2 = obj2.GetWorld();
	float& Ax = world1._41;
	float& Ay = world1._42;
	float& Az = world1._43;
	float& Aw = obj1.m_vBBox.x;
	float& Ah = obj1.m_vBBox.y;
	float& Ad = obj1.m_vBBox.z;
	
	float& Bx = world2._41;
	float& By = world2._42;
	float& Bz = world2._43;
	float& Bw = obj2.m_vBBox.x;
	float& Bh = obj2.m_vBBox.y;
	float& Bd = obj2.m_vBBox.z;

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
	vL[0] = XMVectorSet(v[0].x * obj1.m_vBBox.x, v[0].y * obj1.m_vBBox.x, v[0].z * obj1.m_vBBox.x, 0.0f);
	vL[1] = XMVectorSet(v[1].x * obj1.m_vBBox.y, v[1].y * obj1.m_vBBox.y, v[1].z * obj1.m_vBBox.y, 0.0f);
	vL[2] = XMVectorSet(v[2].x * obj1.m_vBBox.z, v[2].y * obj1.m_vBBox.z, v[2].z * obj1.m_vBBox.z, 0.0f);
	vL[3] = XMVectorSet(v[3].x * obj2.m_vBBox.x, v[3].y * obj2.m_vBBox.x, v[3].z * obj2.m_vBBox.x, 0.0f);
	vL[4] = XMVectorSet(v[4].x * obj2.m_vBBox.y, v[4].y * obj2.m_vBBox.y, v[4].z * obj2.m_vBBox.y, 0.0f);
	vL[5] = XMVectorSet(v[5].x * obj2.m_vBBox.z, v[5].y * obj2.m_vBBox.z, v[5].z * obj2.m_vBBox.z, 0.0f);
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


void Collision::OnCollision(GameObject* obj) {

}


void Collision::SetImGuiVal() {
#if _DEBUG
	ImGui::InputFloat3("m_vCenter",		(float*)&m_vCenter);
	ImGui::InputFloat3("m_vBBox",		(float*)&m_vBBox);
#endif
}


DirectX::XMFLOAT4X4 Collision::GetWorld() {
	XMFLOAT4X4 matrix;
	float3 pos;
	XMStoreFloat3(&pos,
		XMVector3TransformCoord(
			XMLoadFloat3(&m_vCenter),
			XMLoadFloat4x4(&m_gameObject->m_transform->GetMatrix())));
	XMStoreFloat4x4(&matrix, XMMatrixTranslation(
		pos.x, pos.y, pos.z));
	matrix._11 = m_transform->GetMatrix()._11;
	matrix._12 = m_transform->GetMatrix()._12;
	matrix._13 = m_transform->GetMatrix()._13;
	matrix._21 = m_transform->GetMatrix()._21;
	matrix._22 = m_transform->GetMatrix()._22;
	matrix._23 = m_transform->GetMatrix()._23;
	matrix._31 = m_transform->GetMatrix()._31;
	matrix._32 = m_transform->GetMatrix()._32;
	matrix._33 = m_transform->GetMatrix()._33;
	return matrix;
}


// EOF