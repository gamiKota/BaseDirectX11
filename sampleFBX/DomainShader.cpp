#include "DomainShader.h"
#include "D3DClass.h"
#include "Texture.h"
#include "System.h"

DomainShader::DomainShader() {
}


DomainShader::~DomainShader() {
	SAFE_RELEASE(m_pDomainShader);
}


void DomainShader::Bind() {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->DSSetShader(m_pDomainShader, NULL, 0);
}


void DomainShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// シェーダにテクスチャを渡す
	pContext->PSSetShaderResources(0, texNum, ppTex);
}


HRESULT DomainShader::MakeShader(void* pData, UINT size) {
	HRESULT hr = E_FAIL;

	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateDomainShader(pData, size, NULL, &m_pDomainShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}


// EOF