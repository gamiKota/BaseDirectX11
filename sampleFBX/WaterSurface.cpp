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
#include "Shader.h"
#include "System.h"


// シェーダに渡す値
struct SHADER_GLOBAL {
	XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	XMMATRIX	mW;			// ワールド行列(転置行列)
	XMMATRIX	mTex;		// テクスチャ行列(転置行列)
};
struct SHADER_GLOBAL2 {
	XMVECTOR	vEye;		// 視点座標
	// 光源
	XMVECTOR	vLightDir;	// 光源方向
	XMVECTOR	vLa;		// 光源色(アンビエント)
	XMVECTOR	vLd;		// 光源色(ディフューズ)
	XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	XMVECTOR	vAmbient;	// アンビエント色(+テクスチャ有無)
	XMVECTOR	vDiffuse;	// ディフューズ色
	XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	XMVECTOR	vEmissive;	// エミッシブ色
};


/**
 * @constant value
 */
static const DirectX::XMFLOAT4 M_DIFFUSE = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
static const DirectX::XMFLOAT4 M_SPECULAR = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_AMBIENT = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_EMISSIVE = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

static ID3D11Buffer*				g_pConstantBuffer[2];	// 定数バッファ
static ID3D11SamplerState*			g_pSamplerState;		// テクスチャ サンプラ
static ID3D11VertexShader*			g_pVertexShader;		// 頂点シェーダ
static ID3D11InputLayout*			g_pInputLayout;			// 頂点フォーマット
static ID3D11PixelShader*			g_pPixelShader;			// ピクセルシェーダ


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

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	LoadShader("MeshVS", "MeshPS", &g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);

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
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	m_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

	GameObject::Init();
}


void WaterSurface::Uninit() {
	GameObject::Uninit();
}


void WaterSurface::Update() {
	GameObject::Update();
}


void WaterSurface::LastUpdate() {
	GameObject::LastUpdate();
}


void WaterSurface::Draw() {
	// テクスチャマトリックスの初期化
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;
	mtxTexture = XMMatrixIdentity();
	// スケールを反映
	mtxScale = XMMatrixScaling(m_mesh.texSize.x, m_mesh.texSize.y, 1.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);
	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_mesh.texSize.x * m_mesh.texPattern.x, m_mesh.texSize.y * m_mesh.texPattern.y, 0.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);
	// テクスチャマトリックスの設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, mtxTexture);



	//--- 描画
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	Material* pMaterial = new Material();
	MESH* pMesh = &m_mesh;
	Material* material = m_material;
	ID3D11ShaderResourceView* texture = TextureManager::GetInstance().Get(E_TEXTURE::E_TEXTURE_TREE);

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(m_mesh.Zbuff);

	// シェーダ設定
	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(g_pInputLayout);

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &texture);

	SHADER_GLOBAL cb;

	XMMATRIX mtxWorld = XMLoadFloat4x4(&m_transform->GetMatrix());
	cb.mWVP = XMMatrixTranspose(mtxWorld *
		XMLoadFloat4x4(&CCamera::Get()->GetView()) * XMLoadFloat4x4(&CCamera::Get()->GetProj()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->mtxTexture));

	pDeviceContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);

	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&CCamera::Get()->m_transform->m_position);
	Light* light = Light::Get();
	cb2.vLightDir =
		pMesh->light ?
		XMVectorSet(light->m_direction.x, light->m_direction.y, light->m_direction.z, 0.f) : XMVectorSet(0.f, 0.f, 0.f, 0.f);
	cb2.vLa = XMLoadFloat4(&light->m_ambient);
	cb2.vLd = XMLoadFloat4(&light->m_diffuse);
	cb2.vLs = XMLoadFloat4(&light->m_specular);

	cb2.vDiffuse = XMLoadFloat4(&material->m_diffuse);
	cb2.vAmbient = XMVectorSet(material->m_ambient.x, material->m_ambient.y, material->m_ambient.z,
		(texture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(material->m_specular.x, material->m_specular.y, material->m_specular.z, material->m_power);
	cb2.vEmissive = XMLoadFloat4(&material->m_emissive);
	pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);

	// プリミティブ形状をセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);


	//pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);

	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(true);

	SAFE_DELETE(pMaterial);
}


// EOF