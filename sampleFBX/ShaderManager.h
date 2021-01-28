#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>
#include <string>
#include "ShaderBufferManager.h"


enum E_PS {
	E_PS_NORMAL,	// í èÌï`âÊ
	E_PS_FBX,		// HMY-FBX
	E_PS_MAX		// ç≈ëÂíl
};

enum E_VS {
	E_VS_NORMAL,
	E_VS_FBX,
	E_VS_MAX
};

enum E_GS {
	E_GS_NORMAL,
	E_GS_LINE,
	E_GS_MAX,
};



class ShaderManager : public Singleton<ShaderManager> {
public:
	void Initialize();
	void Terminate();

	void BindVS(E_VS shader);
	void BindPS(E_PS shader);
	void BindGS(E_GS shader);

	void SetTexturePS(ID3D11ShaderResourceView* pTex, UINT slot = 0U);

	void UpdateBuffer(std::string bufName, void *data);

private:
	VertexShader*	m_VS[E_VS_MAX];
	GeometryShader* m_GS[E_GS_MAX];
	PixelShader*	m_PS[E_PS_MAX];
};


// EOF