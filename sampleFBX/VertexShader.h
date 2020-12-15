#pragma once


#include "ShaderBase.h"


/**
 * enum 頂点レイアウトの種類を列挙
 */
enum ShaderLayout {
	LAYOUT_FBX,		// HMY-FBX
	LAYOUT_PCUN,	// pos-color-uv-normal
	LAYOUT_PNCU,	// pos-normal-color-uv
	LAYOUT_PUN,		// pos-uv-normal
	LAYOUT_PN,		// pos-normal
	LAYOUT_MAX
};


enum E_SHADER_VS {
	E_SHADER_VS_FBX,
	E_SHADER_VS_OUTLINE,
	E_SHADER_VS_DEFAULT,
	E_SHADER_VS_MAX
};


/**
 * @class 頂点シェーダ
 */
class VertexShader : public ShaderBase
{
public:
	VertexShader(ShaderLayout layout);
	virtual ~VertexShader();

	virtual void Bind();
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

private:
	ShaderLayout m_layout;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
};


// EOF