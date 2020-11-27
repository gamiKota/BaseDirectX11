#pragma once


#include <Windows.h>
#include <d3d11.h>


class ShaderBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	ShaderBase();

	/**
	 * @brief デストラクタ
	 */
	virtual ~ShaderBase();

	/**
	 * @brief シェーダの読み込み、作成
	 */
	HRESULT Create(const char* FileName);

	/**
	 * @brief 適応されるシェーダを決める関数
	 */
	virtual void Bind() = 0;

	/**
	 * @brief テクスチャのセット
	 */
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {}

protected:

	/**
	 * @brief シェーダの作成
	 */
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:
	ID3D11ShaderResourceView* m_pTexture;	//!< テクスチャ
};




/*-----------------------------------------------------------------------------------------
シェーダ設計メモ
あまり融通が利かなくてもいい
シェーダそのものの種類で分ける
VertexShader
GeometryShader
PixelShader
全部連結

例)
モノクロシェーダ → (PixelShaderだけ適用)
モデルライン表示 → (GeometryShaderだけ適用)


-----------------------------------------------------------------------------------------*/



// EOF