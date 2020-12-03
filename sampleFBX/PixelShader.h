#pragma once

#include "ShaderBase.h"


enum E_SHADER_PS {
	E_SHADER_PS_FBX,		// HMY-FBX
	E_SHADER_PS_MONOCHRO,	// ���m�N��
	E_SHADER_PS_PHONG,		// �ӂ���
	E_SHADER_PS_MAX			// �ő�l
};


/**
 * @class �s�N�Z���V�F�[�_
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
	ID3D11ShaderResourceView* m_pTexToon;	//!< �e�N�X�`��
};


// EOF