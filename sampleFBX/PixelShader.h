#pragma once

#include "ShaderBase.h"


/**
 * @class ピクセルシェーダ
 */
class PixelShader : public ShaderBase
{
public:
	PixelShader();
	virtual ~PixelShader();

	virtual void Bind();
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

protected:
	ID3D11PixelShader* m_pPixelShader;
};


// EOF