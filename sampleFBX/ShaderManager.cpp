#include "ShaderManager.h"
#include "ShaderBuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "D3DClass.h"
#include "System.h"


using namespace DirectX;


void ShaderManager::Initialize() {

	HRESULT hr;

	// 頂点シェーダ
	m_VS[E_SHADER_VS_FBX] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_FBX]->Create("data/shader/FbxModelVertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK);}

	m_VS[E_SHADER_VS_OUTLINE] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_OUTLINE]->Create("data/shader/OutLineVS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	m_VS[E_SHADER_VS_DEFAULT] = new VertexShader(LAYOUT_PN);
	hr = m_VS[E_SHADER_VS_DEFAULT]->Create("data/shader/Vertex.cso");
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

	m_PS[E_SHADER_PS_DEFAULT] = new PixelShader;
	hr = m_PS[E_SHADER_PS_DEFAULT]->Create("data/shader/Pixel.cso");
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

	//m_PS[E_SHADER_PS_OUTLINE] = new PixelShader;
	//hr = m_PS[E_SHADER_PS_OUTLINE]->Create("data/shader/OutLinePS.cso");
	//if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }
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
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	switch (shader)
	{
	case E_SHADER_FBX:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_FBX]->Bind();
		pDeviceContext->HSSetShader(NULL, NULL, 0);
		pDeviceContext->DSSetShader(NULL, NULL, 0);
		pDeviceContext->GSSetShader(NULL, NULL, 0);
		pDeviceContext->CSSetShader(NULL, NULL, 0);
		break;

	case E_SHADER_DEFAULT:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_FBX]->Bind();
		pDeviceContext->HSSetShader(NULL, NULL, 0);
		pDeviceContext->DSSetShader(NULL, NULL, 0);
		pDeviceContext->GSSetShader(NULL, NULL, 0);
		pDeviceContext->CSSetShader(NULL, NULL, 0);
		break;

	case E_SHADER_MONOCHROME:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_MONOCHRO]->Bind();
		pDeviceContext->HSSetShader(NULL, NULL, 0);
		pDeviceContext->DSSetShader(NULL, NULL, 0);
		pDeviceContext->GSSetShader(NULL, NULL, 0);
		pDeviceContext->CSSetShader(NULL, NULL, 0);
		break;

	case E_SHADER_PHONG:
		m_VS[E_SHADER_VS_DEFAULT]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_PHONG]->Bind();
		pDeviceContext->HSSetShader(NULL, NULL, 0);
		pDeviceContext->DSSetShader(NULL, NULL, 0);
		pDeviceContext->GSSetShader(NULL, NULL, 0);
		pDeviceContext->CSSetShader(NULL, NULL, 0);
		break;

	case E_SHADER_TOON:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_TOON]->Bind();
		pDeviceContext->HSSetShader(NULL, NULL, 0);
		pDeviceContext->DSSetShader(NULL, NULL, 0);
		pDeviceContext->GSSetShader(NULL, NULL, 0);
		pDeviceContext->CSSetShader(NULL, NULL, 0);
		break;

	case E_SHADER_OUTLINE:
		//m_VS[E_SHADER_VS_OUTLINE]->Bind();
		//m_GS[gs]->Bind();
		//m_PS[E_SHADER_PS_OUTLINE]->Bind();
		break;

	case E_SHADER_MAX:
		break;
	default:
		break;
	}
}


void ShaderManager::UpdateBuffer(std::string bufName, void *data) {
	ShaderBufferManager::GetInstance().Update(bufName, data);
	ShaderBufferManager::GetInstance().Bind(bufName);
}


// EOF