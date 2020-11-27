#pragma once

#include "ShaderBase.h"


enum E_SHADER_PS {
	E_SHADER_PS_FBX,
	E_SHADER_PS_MAX
};


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


/**
 * @class ToonPS
 */
class ToonPS : public PixelShader
{
public:
	ToonPS();
	virtual ~ToonPS();

	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);
protected:

private:
	ID3D11ShaderResourceView* m_pTexToon;	//!< テクスチャ
};


// EOF