#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "ShaderBufferManager.h"


enum E_PS {
	PS_NORMAL,	// 通常描画
	PS_2D,		// 2D
	PS_PHONG,	// フォン
	PS_MAX
};

enum E_VS {
	VS_NORMAL,	// 通常描画
	VS_2D,		// 2D
	VS_MESH,	// メッシュ
	VS_MAX
};

enum E_GS {
	GS_NORMAL,	// 通常描画
	GS_LINE,	// ライン描画
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
	std::vector<VertexShader*>		m_vsList;	//!< 頂点シェーダリスト
	std::vector<PixelShader*>		m_psList;	//!< ピクセルシェーダリスト
	std::vector<GeometryShader*>	m_gsList;	//!< ジオメトリシェーダリスト
};


// EOF