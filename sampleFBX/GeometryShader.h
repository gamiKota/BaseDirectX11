#pragma once
#include "ShaderBase.h"


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