#pragma once
#include "ShaderBase.h"
class DomainShader : public ShaderBase
{
public:
	DomainShader();
	virtual ~DomainShader();

	void Bind();
	void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

protected:
	ID3D11DomainShader* m_pDomainShader;
};

// EOF