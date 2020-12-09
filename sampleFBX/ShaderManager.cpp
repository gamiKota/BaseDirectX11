#include "ShaderManager.h"
#include "ShaderBuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "System.h"


using namespace DirectX;


ShaderBuffer g_worldBuf;		//!< 定数バッファ
DirectX::XMFLOAT4X4 g_world;	//!< 定数バッファに格納されるデータ

ShaderBuffer g_ViewProjBuf;			//!< 定数バッファ
DirectX::XMFLOAT4X4 g_ViewProj[2];	//!< 定数バッファに格納されるデータ


XMFLOAT4 g_value;
ShaderBuffer g_valueBuf;


void ShaderManager::Initialize() {

	HRESULT hr;

	// 頂点シェーダ
	m_VS[E_SHADER_VS_FBX] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_FBX]->Create("data/shader/FbxModelVertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK);}

	m_VS[E_SHADER_VS_OUTLINE] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_OUTLINE]->Create("data/shader/OutLineVS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	m_VS[E_SHADER_VS_DEFAULT] = new VertexShader(LAYOUT_PCUN);
	hr = m_VS[E_SHADER_VS_DEFAULT]->Create("data/shader/VertexShader.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	// ジオメトリシェーダ
	m_GS[E_SHADER_GS_DEFAULT] = new GeometryShader;
	hr = m_GS[E_SHADER_GS_DEFAULT]->Create("data/shader/GeometryShader.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	m_GS[E_SHADER_GS_LINE] = new GeometryShader;
	hr = m_GS[E_SHADER_GS_LINE]->Create("data/shader/LineGS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	// ピクセルシェーダ
	m_PS[E_SHADER_PS_FBX] = new PixelShader;
	hr = m_PS[E_SHADER_PS_FBX]->Create("data/shader/FbxModelPixel.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_MONOCHRO] = new PixelShader;
	hr = m_PS[E_SHADER_PS_MONOCHRO]->Create("data/shader/MonochromePS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_PHONG] = new PixelShader;
	hr = m_PS[E_SHADER_PS_PHONG]->Create("data/shader/PhongPS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_TOON] = new PixelShader;
	hr = m_PS[E_SHADER_PS_TOON]->Create("data/shader/ToonPS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_OUTLINE] = new PixelShader;
	hr = m_PS[E_SHADER_PS_OUTLINE]->Create("data/shader/OutLinePS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	// 定数バッファ
	// シェーダにデータを渡す際には、
	// 必ず16バイトアライメント(float型4つ分)で定数バッファを作成しないといけない
	g_worldBuf.Create(sizeof(g_world));
	g_ViewProjBuf.Create(sizeof(g_ViewProj));
	g_valueBuf.Create(sizeof(g_value));
}


void ShaderManager::Terminate() {
	for (int i = 0; i < E_SHADER_PS_MAX; ++i) {
		SAFE_DELETE(m_PS[i])
	}
	for (int i = 0; i < E_SHADER_GS_MAX; ++i) {
		SAFE_DELETE(m_GS[i])
	}
	for (int i = 0; i < E_SHADER_VS_MAX; ++i) {
		SAFE_DELETE(m_VS[i])
	}
}


void ShaderManager::Bind(E_SHADER shader, E_SHADER_GS gs) {
	switch (shader)
	{
	case E_SHADER_FBX:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_FBX]->Bind();
		break;

	case E_SHADER_MONOCHROME:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_MONOCHRO]->Bind();
		break;

	case E_SHADER_PHONG:
		m_VS[E_SHADER_VS_DEFAULT]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_PHONG]->Bind();
		break;

	case E_SHADER_TOON:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_TOON]->Bind();
		break;

	case E_SHADER_OUTLINE:
		m_VS[E_SHADER_VS_OUTLINE]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_OUTLINE]->Bind();
		break;

	case E_SHADER_MAX:
		break;
	default:
		break;
	}
}


void ShaderManager::UpdateBuffer(XMFLOAT4X4 world) {

	// ワールド座標
	g_world = world;
	// ワールド変換行列を求める
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(0.f, 0.f, 0.f);

	// 行列の並び順が異なるため、シェーダに渡す際に転置を行う
	trans = DirectX::XMMatrixTranspose(trans);
	DirectX::XMStoreFloat4x4(&g_world, trans);

	// ビュー・プロジェクション行列
	g_ViewProj[0] = CCamera::Get()->GetView();
	g_ViewProj[1] = CCamera::Get()->GetProj();

	// カメラ座標
	g_value.x = CCamera::Get()->m_transform->m_position.x;
	g_value.y = CCamera::Get()->m_transform->m_position.y;
	g_value.z = CCamera::Get()->m_transform->m_position.z;
	g_value.w = 1.f;

	
	//--- バッファの更新
	g_worldBuf.UpdateSource(&g_world);
	g_ViewProjBuf.UpdateSource(&g_ViewProj);
	g_valueBuf.UpdateSource(&g_value);

	//--- シェーダに渡す
	// 頂点シェーダ
	g_worldBuf.BindVS(3);
	g_ViewProjBuf.BindVS(4);
	// ピクセルシェーダ
	g_valueBuf.BindPS(5);
}


// EOF