#include "PixelShader.h"
#include "D3DClass.h"
#include "Texture.h"
#include "System.h"

PixelShader::PixelShader() {
}


PixelShader::~PixelShader() {
	SAFE_RELEASE(m_pPixelShader);
}


void PixelShader::Bind() {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->PSSetShader(m_pPixelShader, NULL, 0);
}


void PixelShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// シェーダにテクスチャを渡す
	pContext->PSSetShaderResources(0, texNum, ppTex);
}


HRESULT PixelShader::MakeShader(void* pData, UINT size) {
	HRESULT hr = E_FAIL;

	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreatePixelShader(pData, size, NULL, &m_pPixelShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}




ToonPS::ToonPS()
{
	// テクスチャ読み込み
	CreateTextureFromFile(D3DClass::GetInstance().GetDevice(), "data/texture/Toon.png", &m_pTexToon);
	//LoadTextureFromFile("data/shader/Toon.png", &m_pTexToon);
}


ToonPS::~ToonPS()
{
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pTexToon);
}


void ToonPS::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// シェーダにテクスチャを渡す
	// ID3D11ShaderResourceView変数で送られる情報のうち、
	// おおよそピクセルに欲しい情報は色情報(float4)だから
	// 第一引数のStartSlotは4byt区切りっぽい...？
	pContext->PSSetShaderResources(0, texNum, ppTex);
	pContext->PSSetShaderResources(4, 1, &m_pTexToon);
}

// EOF