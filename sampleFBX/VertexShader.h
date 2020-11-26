#pragma once


#include "ShaderManager.h"


/**
 * @class ���_�V�F�[�_
 */
class VertexShader : public ShaderBase
{
public:
	VertexShader();
	virtual ~VertexShader();

	virtual void Bind();
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11InputLayout* m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
};


// EOF