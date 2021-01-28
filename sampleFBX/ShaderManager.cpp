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
	m_VS[E_VS_FBX] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_VS_FBX]->Create("data/shader/FbxModelVertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK);}

	m_VS[E_VS_NORMAL] = new VertexShader(LAYOUT_PN);
	hr = m_VS[E_VS_NORMAL]->Create("data/shader/Vertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	// ジオメトリシェーダ
	m_GS[E_GS_NORMAL] = new GeometryShader;
	hr = m_GS[E_GS_NORMAL]->Create("data/shader/GeometryShader.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	m_GS[E_GS_LINE] = new GeometryShader;
	hr = m_GS[E_GS_LINE]->Create("data/shader/LineGS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	// ピクセルシェーダ
	m_PS[E_PS_FBX] = new PixelShader;
	hr = m_PS[E_PS_FBX]->Create("data/shader/FbxModelPixel.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_PS_NORMAL] = new PixelShader;
	hr = m_PS[E_PS_NORMAL]->Create("data/shader/Pixel.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }
}


void ShaderManager::Terminate() {
	for (int i = 0; i < E_PS_MAX; ++i) {
		SAFE_DELETE(m_PS[i])
	}
	for (int i = 0; i < E_GS_MAX; ++i) {
		SAFE_DELETE(m_GS[i])
	}
	for (int i = 0; i < E_VS_MAX; ++i) {
		SAFE_DELETE(m_VS[i])
	}
}


void ShaderManager::BindPS(E_PS shader) {
	m_PS[shader]->Bind();
}


void ShaderManager::BindVS(E_VS shader) {
	m_VS[shader]->Bind();
}


void ShaderManager::UpdateBuffer(std::string bufName, void *data) {
	ShaderBufferManager::GetInstance().Update(bufName, data);
	ShaderBufferManager::GetInstance().Bind(bufName);
}


// EOF