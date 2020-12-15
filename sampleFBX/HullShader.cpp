#include "HullShader.h"
#include "D3DClass.h"
#include "Texture.h"
#include "System.h"

HullShader::HullShader() {
}


HullShader::~HullShader() {
	SAFE_RELEASE(m_pHullShader);
}


void HullShader::Bind() {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->HSSetShader(m_pHullShader, NULL, 0);
}


void HullShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// シェーダにテクスチャを渡す
	pContext->PSSetShaderResources(0, texNum, ppTex);
}


HRESULT HullShader::MakeShader(void* pData, UINT size) {
	HRESULT hr = E_FAIL;

	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateHullShader(pData, size, NULL, &m_pHullShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}


// EOF