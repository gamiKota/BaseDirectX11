#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>
#include <string>
#include "ShaderBufferManager.h"


enum E_PS {
	PS_NORMAL,	// �ʏ�`��
	PS_2D,		// 2D
	PS_MAX
};

enum E_VS {
	VS_NORMAL,	// �ʏ�`��
	VS_2D,		// 2D
	VS_MAX
};

enum E_GS {
	GS_NORMAL,	// �ʏ�`��
	GS_LINE,	// ���C���`��
	GS_MAX,
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
	VertexShader*	m_VS[VS_MAX];
	GeometryShader* m_GS[GS_MAX];
	PixelShader*	m_PS[PS_MAX];
};


// EOF