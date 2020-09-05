// 境界ボックス デバッグ表示 [Box.cpp]
#include "Collision.h"
#include "Shader.h"
#include "Camera.h"
#include "FbxModel.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "System.h"

extern CFbxLight	g_light;		// 光源

// 構造体定義
//----- 頂点座標
struct VERTEX {
	XMFLOAT3	position;
	XMFLOAT3	normal;
};
//----- シェーダに渡す値
struct SHADER_GLOBAL {
	XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	XMMATRIX	mW;			// ワールド行列(転置行列)
};
struct SHADER_GLOBAL2 {
	XMVECTOR	vEye;		// 視点座標
	// 光源
	XMVECTOR	vLightDir;	// 光源方向
	XMVECTOR	vLa;		// 光源色(アンビエント)
	XMVECTOR	vLd;		// 光源色(ディフューズ)
	XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	XMVECTOR	vAmbient;	// アンビエント色
	XMVECTOR	vDiffuse;	// ディフューズ色
	XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	XMVECTOR	vEmissive;	// エミッシブ色
};

// コンストラクタ
Collision::Collision() : m_color(1.0f, 1.0f, 1.0f, 0.5f), m_model(E_MODEL_NONE), m_bHit(false) {
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
	// 定数バッファの解放
	for (int i = 0; i < _countof(m_pConstantBuffer); ++i) {
		SAFE_RELEASE(m_pConstantBuffer[i]);
	}
	// ピクセルシェーダ解放
	SAFE_RELEASE(m_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(m_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(m_pVertexShader);
}

// 更新
void Collision::Update()
{
	// 境界ボックス(AABB)の移動
	//XMStoreFloat3(&m_vPosBBox,
	//	XMVector3TransformCoord(
	//		XMLoadFloat3(&m_vCenter),
	//		XMLoadFloat4x4(&m_gameObject->m_transform)));
	//XMFLOAT4X4 matrix;
	//XMStoreFloat4x4(&matrix, XMMatrixTranslation(
	//	m_vPosBBox.x, m_vPosBBox.y, m_vPosBBox.z));
	//SetWorld(matrix);
}

// 描画
void Collision::Draw() {
#if _DEBUG

	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 背面カリング(裏を描かない)

	// 境界ボックスの色
	if (m_bHit) {
		XMFLOAT4 vRed(1.0f, 0.0f, 0.0f, 0.5f);
		SetColor(&vRed);
	}
	else {
		XMFLOAT4 vGreen(0.0f, 1.0f, 0.0f, 0.5f);
		SetColor(&vGreen);
	}

	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	// プリミティブ形状をセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファ設定
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// 定数バッファ設定
	SHADER_GLOBAL cb;
	XMMATRIX mtxWorld = XMLoadFloat4x4(m_transform);
	CCamera* pCamera = CCamera::Get();
	cb.mWVP = XMMatrixTranspose(mtxWorld *
		XMLoadFloat4x4(&pCamera->GetView()) *
		XMLoadFloat4x4(&pCamera->GetProj()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetEye());
	CFbxLight& light = g_light;
	cb2.vLightDir = XMVectorSet(light.m_direction.x, light.m_direction.y, light.m_direction.z, 0.f);
	cb2.vLa = XMLoadFloat4(&light.m_ambient);
	cb2.vLd = XMLoadFloat4(&light.m_diffuse);
	cb2.vLs = XMLoadFloat4(&light.m_specular);
	cb2.vDiffuse = XMLoadFloat4(&m_color);
	cb2.vAmbient = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	cb2.vSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	cb2.vEmissive = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);
	// 描画
	pDeviceContext->DrawIndexed(36, 0, 0);

	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);	// 前面カリング(表を描かない)
	D3DClass::GetInstance().SetZWrite(true);

#endif
}


void Collision::SetWorld(XMFLOAT4X4 world) {
	*m_transform = world;
}


void Collision::Init(E_MODEL model) {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	if (model != E_MODEL_NONE && model != E_MODEL_MAX) {
		// 境界ボックス初期化
		m_model = model;
		m_vCenter = ModelManager::GetInstance().Get(m_model)->GetCenter();
		m_vBBox = ModelManager::GetInstance().Get(m_model)->GetBBox();
		m_vPosBBox = m_vCenter;
	}

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = LoadShader("Vertex", "Pixel",
		&m_pVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[0]);
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[1]);

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
	float& Ax = obj1.m_transform->_41;
	float& Ay = obj1.m_transform->_42;
	float& Az = obj1.m_transform->_43;
	float& Aw = obj1.m_vBBox.x;
	float& Ah = obj1.m_vBBox.y;
	float& Ad = obj1.m_vBBox.z;
	
	float& Bx = obj2.m_transform->_41;
	float& By = obj2.m_transform->_42;
	float& Bz = obj2.m_transform->_43;
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

	//// 境界ボックスの色
	//if (hit) {
	//	XMFLOAT4 vRed(1.0f, 0.0f, 0.0f, 0.5f);
	//	m_box.SetColor(&vRed);
	//	CPlayer::SetHit(true);
	//} else {
	//	XMFLOAT4 vGreen(0.0f, 1.0f, 0.0f, 0.5f);
	//	m_box.SetColor(&vGreen);
	//}
}


// EOF