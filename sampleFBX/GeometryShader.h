#pragma once
#include "ShaderBase.h"


enum E_SHADER_GS {
	E_SHADER_GS_DEFAULT,
	E_SHADER_GS_LINE,
	E_SHADER_GS_MAX,
};


/**
 * @class GeometryShader
 */
class GeometryShader : public ShaderBase {
public:
	GeometryShader();
	virtual ~GeometryShader();

	virtual void Bind();
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);

protected:
	virtual HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11GeometryShader* m_pGeometryShader;
};

// EOF