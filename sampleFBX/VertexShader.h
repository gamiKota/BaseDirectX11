#pragma once


#include "ShaderBase.h"


/**
 * enum 頂点レイアウトの種類を列挙
 */
enum ShaderLayout {
	LAYOUT_PCUN,	// pos-color-uv-normal
	LAYOUT_PCU,		// pos-color-uv
	LAYOUT_PN,		// pos-normal
	LAYOUT_MAX
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