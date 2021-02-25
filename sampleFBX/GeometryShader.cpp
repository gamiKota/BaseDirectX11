#include "GeometryShader.h"
#include "D3DClass.h"
#include "System.h"


GeometryShader::GeometryShader() : m_pGeometryShader(NULL) {
}


GeometryShader::~GeometryShader() {
	SAFE_RELEASE(m_pGeometryShader);
}


void GeometryShader::Bind() {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->GSSetShader(m_pGeometryShader, NULL, 0);
}


void GeometryShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// �V�F�[�_�Ƀe�N�X�`����n��
	pContext->GSSetShaderResources(0, texNum, ppTex);
}


HRESULT GeometryShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr = E_FAIL;

	// ���_���C�A�E�g�쐬
	// CPU-GPU�ԂŃf�[�^���s���������邽��
	ID3D11Device *pDevice = D3DClass::GetInstance().GetDevice();

	// ���_�V�F�[�_�̍쐬
	hr = pDevice->CreateGeometryShader(pData, size, NULL, &m_pGeometryShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}


// EOF