/**
 * @file VertexShader.cpp
 */


/**
 * @include
 */
#include "VertexShader.h"
#include "D3DClass.h"
#include "System.h"


VertexShader::VertexShader(ShaderLayout layout) : m_layout(layout), m_pInputLayout(nullptr) {
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
	// ①頂点フォーマットの定義、生成
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_INDEX",  0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// ②pos-color-uv-normal
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

	// ②pos-color-uv-normal
	static const D3D11_INPUT_ELEMENT_DESC PosNormalColorUV[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// ③pos-uv-normal
	static const D3D11_INPUT_ELEMENT_DESC PosUvNormal[] = {
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

	// ④pos-normal
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[] = {
		{
			"POSITION", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}, {
			"NORMAL", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// 頂点レイアウト作成
	const D3D11_INPUT_ELEMENT_DESC* pDesc[] = { layout, PosColorUvNormal, PosNormalColorUV ,PosUvNormal, PosNormal };
	int dataNum[] = { _countof(layout), _countof(PosColorUvNormal), _countof(PosNormalColorUV) , _countof(PosUvNormal), _countof(PosNormal) };

	// CPU-GPU間でデータを行き来させるため
	ID3D11Device *pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateInputLayout(pDesc[m_layout], dataNum[m_layout], pData, size, &m_pInputLayout);
	if (FAILED(hr)) { return hr; }

	// 頂点シェーダの作成
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVertexShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}

// EOF