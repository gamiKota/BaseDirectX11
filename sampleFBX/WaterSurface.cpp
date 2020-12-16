/**
 * @file WaterSurface.cpp
 */


/**
 * @include
 */
#include "WaterSurface.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Collision.h"
#include "polygon.h"
#include "Light.h"
#include "input.h"
#include "GameObjectMesh.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Frame.h"
#include "imgui.h"
#include "Drop.h"
#include "System.h"



// 頂点定義
struct _VERTEX
{
	XMFLOAT3 pos;    // 頂点座標
	XMFLOAT2 texel;  // テクセル
};

// 定数定義
typedef struct
{
	XMMATRIX  g_matWVP; // ワールド × ビュー × 射影 行列
} _CBUFFER0;

typedef struct
{
	float g_fTessFactor;		// ポリゴンのエッジのテッセレーション係数
	float g_fInsideTessFactor;	// ポリゴン内部のテッセレーション係数
	// 16バイト調整用変数
	float g_Dummy2;
	float g_Dummy3;
} _CBUFFER1;

typedef struct
{
	float g_timer;		//!< 経過時間
	float g_Dummy1;
	float g_Dummy2;
	float g_Dummy3;
} _CBUFFER2;


typedef struct
{
	float g_amplitude;	//!< 影響力
	float g_Dummy1;
	float g_Dummy2;
	float g_Dummy3;
} _CBUFFER3;


typedef struct
{
	float3 g_collisionPos;	//!< 当たった座標
	float g_Dummy1;
} _CBUFFER4;


/**
 * @constant value
 */
static const DirectX::XMFLOAT4 M_DIFFUSE = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
static const DirectX::XMFLOAT4 M_SPECULAR = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_AMBIENT = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_EMISSIVE = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

static ID3D11Buffer*				g_pConstantBuffer[5];	// 定数バッファ
static ID3D11SamplerState*			g_pSamplerState;		// テクスチャ サンプラ
static ID3D11VertexShader*			g_pVertexShader;		// 頂点シェーダ
static ID3D11InputLayout*			g_pInputLayout;			// 頂点フォーマット
//static ID3D11PixelShader*			g_pPixelShader;			// ピクセルシェーダ


static HullShader* g_HS;
static DomainShader* g_DS;
static GeometryShader* g_GS;
static PixelShader* g_PS;

// 波シェーダの時はなるべく同じ値の方が奇麗に見える
static float TessFactor = 32.0f;			// ポリゴンのエッジのテッセレーション係数の定数
static float InsideTessFactor = 32.0f;		// ポリゴン内部のテッセレーション係数の定数
static float Timer = 0.f;


WaterSurface::WaterSurface() : GameObject("WaterSurface") {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();

	// マテリアルの初期設定
	m_material->m_diffuse = M_DIFFUSE;
	m_material->m_ambient = M_AMBIENT;
	m_material->m_specular = M_SPECULAR;
	m_material->m_power = 50.0f;
	m_material->m_emissive = M_EMISSIVE;

	// テクスチャ設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
}


WaterSurface::WaterSurface(std::string name, std::string tag) : GameObject(name, tag) {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();

	// マテリアルの初期設定
	m_material->m_diffuse = M_DIFFUSE;
	m_material->m_ambient = M_AMBIENT;
	m_material->m_specular = M_SPECULAR;
	m_material->m_power = 50.0f;
	m_material->m_emissive = M_EMISSIVE;

	// テクスチャ設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
}


WaterSurface::~WaterSurface() {

}


void WaterSurface::Init() {
	// メッシュの初期化(初期設定)
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr;

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//LoadShader("MeshVS", "WaterSurfacePS", &g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));
	LoadVertexShader("WaterSurfaceVS", &g_pVertexShader, &g_pInputLayout,layout, _countof(layout));


	//g_VS = new VertexShader(ShaderLayout::LAYOUT_PNCU);
	//if (FAILED(g_VS->Create("data/shader/MeshVS.cso"))) { MessageBox(NULL, L"VS", NULL, MB_OK); }
	g_HS = new HullShader();
	if (FAILED(g_HS->Create("data/shader/WaterSurfaceHS.cso"))) { MessageBox(NULL, L"HS", NULL, MB_OK); }
	g_DS = new DomainShader();
	if (FAILED(g_DS->Create("data/shader/WaterSurfaceDS.cso"))) { MessageBox(NULL, L"DS", NULL, MB_OK); }
	g_GS = new GeometryShader();
	if (FAILED(g_GS->Create("data/shader/LineGS.cso"))) { MessageBox(NULL, L"GS", NULL, MB_OK); }
	g_PS = new PixelShader();
	if (FAILED(g_PS->Create("data/shader/WaterSurfacePS.cso"))) { MessageBox(NULL, L"PS", NULL, MB_OK); }

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(_CBUFFER0);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	bd.ByteWidth = sizeof(_CBUFFER1);
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);
	bd.ByteWidth = sizeof(_CBUFFER2);
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[2]);

	bd.ByteWidth = sizeof(_CBUFFER3);
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[3]);
	if (FAILED(hr)) { MessageBox(NULL, L"バッファ", NULL, MB_OK); }
	bd.ByteWidth = sizeof(_CBUFFER4);
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[4]);
	if (FAILED(hr)) { MessageBox(NULL, L"バッファ", NULL, MB_OK); }


	//// 定数バッファ生成
	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(SHADER_GLOBAL);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = 0;
	//pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	//bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	//pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&sd, &g_pSamplerState);

	// 頂点情報の作成
	m_mesh.nNumVertex = 4;
	_VERTEX* pVertexWk = new _VERTEX[m_mesh.nNumVertex];
	pVertexWk[0].pos = XMFLOAT3( 0.5f,  0.5f, 0.0f);
	pVertexWk[1].pos = XMFLOAT3(-0.5f,  0.5f, 0.0f);
	pVertexWk[2].pos = XMFLOAT3( 0.5f, -0.5f, 0.0f);
	pVertexWk[3].pos = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	//pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].texel = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[1].texel = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[2].texel = XMFLOAT2(1.0f, 1.0f);
	pVertexWk[3].texel = XMFLOAT2(0.0f, 1.0f);
	m_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	//MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(_VERTEX) * m_mesh.nNumVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pVertexWk;
	hr = pDevice->CreateBuffer(&vbd, &initData, &m_mesh.pVertexBuffer);
	if (FAILED(hr)) MessageBox(NULL, NULL, NULL, MB_OK);
	CD3D11_BUFFER_DESC ibd(m_mesh.nNumIndex * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = pIndexWk;
	hr = pDevice->CreateBuffer(&ibd, &initData, &m_mesh.pIndexBuffer);
	if (FAILED(hr)) MessageBox(NULL, NULL, NULL, MB_OK);


	delete[] pIndexWk;
	delete[] pVertexWk;

	m_dropList = nullptr;
	//m_dropList.clear();

	GameObject::Init();
}


void WaterSurface::Uninit() {
	SAFE_RELEASE(g_pSamplerState);
	SAFE_RELEASE(g_pConstantBuffer[4]);
	SAFE_RELEASE(g_pConstantBuffer[3]);
	SAFE_RELEASE(g_pConstantBuffer[2]);
	SAFE_RELEASE(g_pConstantBuffer[1]);
	SAFE_RELEASE(g_pConstantBuffer[0]);
	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pInputLayout);
	//SAFE_RELEASE(g_pPixelShader);

	SAFE_DELETE(g_PS);
	SAFE_DELETE(g_GS);
	SAFE_DELETE(g_DS);
	SAFE_DELETE(g_HS);

	GameObject::Uninit();
}


void WaterSurface::Update() {

	if (Input::isTrigger('D')) {
		GameObjectMesh* obj = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE::E_TEXTURE_TREE, "Drop", "Drop");
		GameObject::Instantiate(obj);
		obj->m_transform->m_position = float3(0.f, 100.f, 0.f);
		obj->m_transform->m_scale = float3(10.f, 10.f, 10.f);

		m_dropList = obj->AddComponent<Drop>();
		//m_dropList.push_back(obj->AddComponent<Drop>());

		//obj->m_transform->m_position.x = (float)GetRandom(-200, 200);
		//obj->m_transform->m_position.z = (float)GetRandom(-200, 200);
	}

	GameObject::Update();
}


void WaterSurface::LastUpdate() {
	GameObject::LastUpdate();
}


void WaterSurface::Draw() {
	ImGui::DragFloat("Edge", &TessFactor, 0.1f);
	ImGui::DragFloat("Inpolygon", &InsideTessFactor, 0.1f);
	ImGui::DragFloat("timer", &Timer, 0.1f);

	//// テクスチャマトリックスの初期化
	//XMMATRIX mtxTexture, mtxScale, mtxTranslate;
	//mtxTexture = XMMatrixIdentity();
	//// スケールを反映
	//mtxScale = XMMatrixScaling(m_mesh.texSize.x, m_mesh.texSize.y, 1.0f);
	//mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);
	//// 移動を反映
	//mtxTranslate = XMMatrixTranslation(m_mesh.texSize.x * m_mesh.texPattern.x, m_mesh.texSize.y * m_mesh.texPattern.y, 0.0f);
	//mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);
	//// テクスチャマトリックスの設定
	//XMStoreFloat4x4(&m_mesh.mtxTexture, mtxTexture);



	//--- 描画
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	Material* pMaterial = new Material();
	MESH* pMesh = &m_mesh;
	Material* material = m_material;
	ID3D11ShaderResourceView* texture = TextureManager::GetInstance().Get(E_TEXTURE::E_TEXTURE_TREE);


	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(false);


	//pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	//g_HS->Bind();
	//g_DS->Bind();
	//g_PS->Bind();

	// 頂点バッファをセット
	UINT stride = sizeof(_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);

	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// レイアウトの設定
	pDeviceContext->IASetInputLayout(g_pInputLayout);

	// メッシュが4つの頂点(コントロールポイント)を持つと指定する
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダをデバイスに設定する
	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);

	// ハルシェーダをデバイスに設定する
	g_HS->Bind();
	
	// ドメインシェーダをデバイスに設定する
	g_DS->Bind();

	// ジオメトリーシェーダを無効にする
	//pDeviceContext->GSSetShader(NULL, NULL, 0);
	g_GS->Bind();

	// ピクセルシェーダをデバイスに設定する
	g_PS->Bind();

	// コンピュートシェーダの無効
	pDeviceContext->CSSetShader(NULL, NULL, 0);

	// 行列を定数バッファにセット
	{
		_CBUFFER0 cbuffer0;
		XMMATRIX mtxWorld = XMLoadFloat4x4(&m_transform->GetMatrix());
		cbuffer0.g_matWVP = XMMatrixTranspose(mtxWorld *
			XMLoadFloat4x4(&CCamera::Get()->GetView()) * XMLoadFloat4x4(&CCamera::Get()->GetProj()));
		pDeviceContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cbuffer0, 0, 0);
	}
	
	// テッセレーション係数を定数バッファにセット
	{
		_CBUFFER1 cbuffer1;
		cbuffer1.g_fTessFactor = TessFactor;
		cbuffer1.g_fInsideTessFactor = InsideTessFactor;
		pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cbuffer1, 0, 0);
	}

	// 波を定数バッファにセット
	{
		_CBUFFER2 cbuffer2;
		Timer += Frame::GetInstance().GetDeltaTime();
		cbuffer2.g_timer = Timer;
		pDeviceContext->UpdateSubresource(g_pConstantBuffer[2], 0, nullptr, &cbuffer2, 0, 0);
	}

	// 振幅
	{
		_CBUFFER3 cbuffer3;
		if (m_dropList != nullptr && m_dropList->m_isCollsion) {
			cbuffer3.g_amplitude = m_dropList->m_influence;
		}
		else {
			cbuffer3.g_amplitude = 0.f;
		}
		pDeviceContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cbuffer3, 0, 0);
	}

	// 座標
	{
		_CBUFFER4 cbuffer4;
		if (m_dropList != nullptr && m_dropList->m_isCollsion) {
			cbuffer4.g_collisionPos = m_dropList->m_transform->m_position;
		}
		else {
			cbuffer4.g_collisionPos = float3();
		}

		//XMVECTOR light = XMVectorSet(0.0f, 2.0f, -1.5f, 0.0f);
		//XMVECTOR attenuation = XMVectorSet(1.0f, 0.0f, 0.2f, 0.0f);
		//
		//ConstantBuffer cb;
		//XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
		//XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
		//XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
		//XMStoreFloat4(&cb.light, light);

		pDeviceContext->UpdateSubresource(g_pConstantBuffer[4], 0, nullptr, &cbuffer4, 0, 0);
	}
	
	// ハルシェーダーに定数バッファを設定する。
	pDeviceContext->HSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);
	
	// ドメインシェーダーに定数バッファを設定する。
	pDeviceContext->DSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
	pDeviceContext->DSSetConstantBuffers(2, 1, &g_pConstantBuffer[2]);
	pDeviceContext->DSSetConstantBuffers(3, 1, &g_pConstantBuffer[3]);
	pDeviceContext->DSSetConstantBuffers(4, 1, &g_pConstantBuffer[4]);

	// ピクセルシェーダにサンプルステートを渡す
	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	// ピクセルシェーダにテクスチャを渡す
	pDeviceContext->PSSetShaderResources(0, 1, &texture);



	////--- バッファの設定
	//SHADER_GLOBAL cb;
	//
	//XMMATRIX mtxWorld = XMLoadFloat4x4(&m_transform->GetMatrix());
	//cb.mWVP = XMMatrixTranspose(mtxWorld *
	//	XMLoadFloat4x4(&CCamera::Get()->GetView()) * XMLoadFloat4x4(&CCamera::Get()->GetProj()));
	//cb.mW = XMMatrixTranspose(mtxWorld);
	//cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->mtxTexture));
	//
	//pDeviceContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	//pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
	//
	//SHADER_GLOBAL2 cb2;
	//cb2.vEye = XMLoadFloat3(&CCamera::Get()->m_transform->m_position);
	//Light* light = Light::Get();
	//cb2.vLightDir =
	//	pMesh->light ?
	//	XMVectorSet(light->m_direction.x, light->m_direction.y, light->m_direction.z, 0.f) : XMVectorSet(0.f, 0.f, 0.f, 0.f);
	//cb2.vLa = XMLoadFloat4(&light->m_ambient);
	//cb2.vLd = XMLoadFloat4(&light->m_diffuse);
	//cb2.vLs = XMLoadFloat4(&light->m_specular);
	//
	//cb2.vDiffuse = XMLoadFloat4(&material->m_diffuse);
	//cb2.vAmbient = XMVectorSet(material->m_ambient.x, material->m_ambient.y, material->m_ambient.z,
	//	(texture != nullptr) ? 1.f : 0.f);
	//cb2.vSpecular = XMVectorSet(material->m_specular.x, material->m_specular.y, material->m_specular.z, material->m_power);
	//cb2.vEmissive = XMLoadFloat4(&material->m_emissive);
	//pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	//pDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);

	// プリミティブ形状をセット
	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);
	//pDeviceContext->Draw(4, 0);

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);

	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(true);

	SAFE_DELETE(pMaterial);
}



void WaterSurface::SetImGuiVal() {


}



// EOF