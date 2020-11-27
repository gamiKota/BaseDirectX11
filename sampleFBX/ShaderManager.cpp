#include "ShaderManager.h"
#include "System.h"


void ShaderManager::Initialize() {

	HRESULT hr;

	// 頂点シェーダ
	m_VS[E_SHADER_VS_FBX] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_FBX]->Create("data/shader/FbxModelVertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK);}

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
		m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_FBX]->Bind();
		break;

	case E_SHADER_MONOCHROME:
		m_VS[E_SHADER_VS_FBX]->Bind();
		m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_MONOCHRO]->Bind();
		break;

	case E_SHADER_MAX:
		break;
	default:
		break;
	}
}


// EOF