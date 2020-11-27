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
	// �V�F�[�_�Ƀe�N�X�`����n��
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
	// �e�N�X�`���ǂݍ���
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
	// �V�F�[�_�Ƀe�N�X�`����n��
	// ID3D11ShaderResourceView�ϐ��ő�������̂����A
	// �����悻�s�N�Z���ɗ~�������͐F���(float4)������
	// ��������StartSlot��4byt��؂���ۂ�...�H
	pContext->PSSetShaderResources(0, texNum, ppTex);
	pContext->PSSetShaderResources(4, 1, &m_pTexToon);
}

// EOF