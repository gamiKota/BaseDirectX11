/**
 * @file VertexShader.cpp
 */


/**
 * @include
 */
#include "VertexShader.h"
#include "D3DClass.h"
#include "System.h"


VertexShader::VertexShader()
	: m_pInputLayout(nullptr)
{
}

VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->IASetInputLayout(m_pInputLayout);
	pContext->VSSetShader(m_pVertexShader, NULL, 0);

}

void VertexShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {
	ID3D11DeviceContext * pContext = D3DClass::GetInstance().GetDeviceContext();
	// シェーダにテクスチャを渡す
	pContext->VSSetShaderResources(0, texNum, ppTex);
}

HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr = E_FAIL;

	// 頂点シェーダがバッファのデータ構造がどのようになっているかを
	// 知るために、頂点レイアウトのデータを作成する
	static const D3D11_INPUT_ELEMENT_DESC PosColorUvNormal[] = {
		{
			// セマンティクス
			// 頂点の使用方法を表す
			"POSITION", 0,
			// フォーマット
			DXGI_FORMAT_R32G32B32_FLOAT,
			// データの返し位置とオフセット位置
			0, 0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},  {
			"COLOR", 0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			// 直前のデータのすぐ後ろ指定(勝手にバイト数を計算してくれる)
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}, {
			"TEXCOORD", 0,
			DXGI_FORMAT_R32G32_FLOAT,
			// 直前のデータのすぐ後ろ指定(勝手にバイト数を計算してくれる)
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}, {
			"NORMAL", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// 頂点レイアウト作成
	// CPU-GPU間でデータを行き来させるため
	ID3D11Device *pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateInputLayout(PosColorUvNormal, _countof(PosColorUvNormal), pData, size, &m_pInputLayout);
	if (FAILED(hr)) { return hr; }

	// 頂点シェーダの作成
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVertexShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}