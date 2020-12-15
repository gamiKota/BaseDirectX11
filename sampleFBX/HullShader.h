#pragma once
#include "ShaderBase.h"
class HullShader : public ShaderBase
{
public:
	HullShader();
	virtual ~HullShader();

	void Bind();
	void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

protected:
	ID3D11HullShader* m_pHullShader;
};

// EOF