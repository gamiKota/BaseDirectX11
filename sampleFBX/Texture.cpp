#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdlib.h>
#include "Texture.h"
#include "D3DClass.h"

#pragma comment(lib, "DirectXTex")

#include "System.h"

using namespace DirectX;

// メモリから生成
HRESULT CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
	_In_bytecount_(wicDataSize) const uint8_t* wicData,
	_In_ size_t wicDataSize,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_Out_opt_ TexMetadata* pTexInfo)
{
	*textureView = nullptr;
	TexMetadata meta;
	ScratchImage image;
	HRESULT hr;
	if (wicDataSize >= 18 && memcmp(&wicData[wicDataSize - 18], "TRUEVISION-XFILE.", 18) == 0) {
		hr = LoadFromTGAMemory(wicData, wicDataSize, &meta, image);
	} else if (wicDataSize >= 4 && memcmp(wicData, "DDS ", 4) == 0) {
		hr = LoadFromDDSMemory(wicData, wicDataSize, 0, &meta, image);
	} else {
		hr = LoadFromWICMemory(wicData, wicDataSize, 0, &meta, image);
	}
	if (FAILED(hr)) return hr;
	if (pTexInfo) *pTexInfo = meta;
	return CreateShaderResourceView(d3dDevice, image.GetImages(), image.GetImageCount(), meta, textureView);
}

// ファイルから生成
HRESULT CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
	_In_z_ const wchar_t* szFileName,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_Out_opt_ TexMetadata* pTexInfo)
{
	*textureView = nullptr;
	TexMetadata meta;
	ScratchImage image;
	WCHAR wszExt[_MAX_EXT];
	_wsplitpath(szFileName, nullptr, nullptr, nullptr, wszExt);
	HRESULT hr;
	if (_wcsicmp(wszExt, L".tga") == 0)
		hr = LoadFromTGAFile(szFileName, &meta, image);
	else if (_wcsicmp(wszExt, L".dds") == 0)
		hr = LoadFromDDSFile(szFileName, 0, &meta, image);
	else
		hr = LoadFromWICFile(szFileName, 0, &meta, image);
	if (FAILED(hr)) return hr;
	if (pTexInfo) *pTexInfo = meta;
	return CreateShaderResourceView(d3dDevice, image.GetImages(), image.GetImageCount(), meta, textureView);
}

HRESULT CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
	_In_z_ const char* szFileName,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_Out_opt_ TexMetadata* pTexInfo)
{
	WCHAR wszTexFName[_MAX_PATH];
	int nLen = MultiByteToWideChar(CP_ACP, 0, szFileName, lstrlenA(szFileName), wszTexFName, _countof(wszTexFName));
	if (nLen <= 0) return E_FAIL;
	wszTexFName[nLen] = L'\0';
	return CreateTextureFromFile(d3dDevice, wszTexFName, textureView, pTexInfo);
}



HRESULT LoadTextureFromFile(const char *pszFileName, ID3D11ShaderResourceView **ppTexture)
{
	HRESULT hr;
	ID3D11Device *pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext *pContext = D3DClass::GetInstance().GetDeviceContext();
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;

	// 文字変換
	MultiByteToWideChar(0, 0, pszFileName, -1, wPath, MAX_PATH);
	// ファイル別読み込み
	if (strstr(pszFileName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	}
	if (FAILED(hr))
	{
		MessageBoxA(NULL, pszFileName, "Failed to load texture.", MB_OK);
		return hr;
	}

	// シェーダリソースビュー作成
	hr = CreateShaderResourceView(
		pDevice,
		image.GetImages(), image.GetImageCount(),
		mdata, ppTexture);

	return hr;
}

HRESULT CreateRenderTexture(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11ShaderResourceView** ppTexture, ID3D11RenderTargetView** ppRTV)
{
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	//--- テクスチャ
	// 設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	// 生成
	ID3D11Texture2D* tex2D;
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &tex2D);
	if (FAILED(hr)) { return E_FAIL; }

	//--- レンダーターゲット
	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// 生成
	hr = pDevice->CreateRenderTargetView(tex2D, &rtvDesc, ppRTV);
	if (FAILED(hr)) { return E_FAIL; }

	//--- シェーダリソース
	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// 生成
	hr = pDevice->CreateShaderResourceView(tex2D, &srvDesc, ppTexture);
	if (FAILED(hr)) { return E_FAIL; }

	return hr;
}


HRESULT CreateDepthStencil(UINT width, UINT height, DXGI_FORMAT format,
	ID3D11DepthStencilView** ppDSV)
{
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	//--- テクスチャ
	// 設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	// 生成
	ID3D11Texture2D* tex2D;
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &tex2D);
	if (FAILED(hr)) { return E_FAIL; }

	//--- デプスステンシル
	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	// 生成
	hr = pDevice->CreateDepthStencilView(tex2D, &dsvDesc, ppDSV);
	if (FAILED(hr)) { return E_FAIL; }

	return hr;
}