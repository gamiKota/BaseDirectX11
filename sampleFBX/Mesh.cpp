//=============================================================================
//
// メッシュ共通処理 [mesh.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Mesh.h"
#include "camera.h"
#include "light.h"
#include "Shader.h"
#include "Texture.h"
#include "ShaderBuffer.h"
#include "Transform.h"
#include "imgui.h"
#include "System.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
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

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer*				g_pConstantBuffer[2];	// 定数バッファ
static ID3D11SamplerState*			g_pSamplerState;		// テクスチャ サンプラ
static ID3D11VertexShader*			g_pVertexShader;		// 頂点シェーダ
static ID3D11InputLayout*			g_pInputLayout;			// 頂点フォーマット
static ID3D11PixelShader*			g_pPixelShader;			// ピクセルシェーダ


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMesh(void)
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr;

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = LoadShader("MeshVS", "MeshPS",
		&g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	if (FAILED(hr)) return hr;
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &g_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitMesh(void)
{
	// テクスチャ サンプラの開放
	SAFE_RELEASE(g_pSamplerState);
	// 定数バッファの解放
	for (int i = 0; i < _countof(g_pConstantBuffer); ++i) {
		SAFE_RELEASE(g_pConstantBuffer[i]);
	}
	// ピクセルシェーダ解放
	SAFE_RELEASE(g_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(g_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(g_pVertexShader);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawMesh(MESH* pMesh, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4X4* matrix)
{
	if (!pMesh->isDraw)	return;

	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(pMesh->Zbuff);

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


	pDeviceContext->HSSetShader(NULL, NULL, 0);
	pDeviceContext->DSSetShader(NULL, NULL, 0);
	pDeviceContext->GSSetShader(NULL, NULL, 0);
	pDeviceContext->CSSetShader(NULL, NULL, 0);


	SHADER_GLOBAL cb;

	XMMATRIX mtxWorld = XMLoadFloat4x4(matrix);
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

	pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);

	//// プリミティブ形状をセット
	//static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
	//	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	//};
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);

	// 背面カリング (通常は表面のみ描画)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);


	//pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);
	
	// Zバッファ無効
	D3DClass::GetInstance().SetZBuffer(true);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh,
	VERTEX_3D vertexWk[], int indexWk[])
{
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX_3D) * pMesh->nNumVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertexWk;
	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &pMesh->pVertexBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	CD3D11_BUFFER_DESC ibd(pMesh->nNumIndex * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indexWk;
	hr = pDevice->CreateBuffer(&ibd, &initData, &pMesh->pIndexBuffer);

	return hr;
}


//=============================================================================
// メッシュ解放
//=============================================================================
void ReleaseMesh(MESH* pMesh)
{
	if (!pMesh) return;
	// 頂点バッファ解放
	SAFE_RELEASE(pMesh->pVertexBuffer);
	// インデックス バッファ解放
	SAFE_RELEASE(pMesh->pIndexBuffer);
}


// ROF