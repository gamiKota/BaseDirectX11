#include "DrawBuffer.h"
#include "D3DClass.h"
#include "System.h"

DrawBuffer::DrawBuffer()
	: m_pVertexBuffer(NULL)
	, m_vtxSize(0)
	, m_vtxCount(0)
	, m_pIndexBuffer(NULL)
	, m_idxSize(0)
	, m_idxCount(0)
	, m_pInputBuffer(NULL)
	, m_iptSize(0)
	, m_iptCount(0)
{
}
DrawBuffer::~DrawBuffer()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

HRESULT DrawBuffer::CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite)
{
	//--- 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	// バッファの大きさ
	bufDesc.ByteWidth =
		size * count;
	// バッファの使用方法
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	// バッファの利用先
	bufDesc.BindFlags =
		D3D11_BIND_VERTEX_BUFFER;
	// 書き込み指定
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	//--- バッファの初期値を設定
	D3D11_SUBRESOURCE_DATA
		subResource = {};
	// 初期化に使うデータの置き場所
	// の指定
	subResource.pSysMem = pVtx;

	//--- 頂点バッファの作成
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(
		&bufDesc, &subResource,
		&m_pVertexBuffer);

	//--- そのほかの情報を設定
	if (SUCCEEDED(hr))
	{
		m_vtxSize = size;
		m_vtxCount = count;
	}
	return hr;
}

HRESULT DrawBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// インデックスバッファ生成
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr;
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pIndexBuffer);
	if (SUCCEEDED(hr))
	{
		m_idxSize = size;
		m_idxCount = count;
	}

	return hr;
}

HRESULT DrawBuffer::CreateInputBuffer(UINT size, UINT count) {
//HRESULT DrawBuffer::CreateInputBuffer() {

	// 頂点バッファ インスタンシング用 作成
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	D3D11_BUFFER_DESC bufDesc = {};
	HRESULT hr;

	m_iptSize = size;
	m_iptCount = count;

	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.ByteWidth = m_iptSize * m_iptCount;
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufDesc.MiscFlags = 0;
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pInputBuffer);

	return hr;
}

void DrawBuffer::Draw(
	D3D11_PRIMITIVE_TOPOLOGY primitive, int num)
{
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	// 頂点一つ当たりの大きさ
	UINT stride = m_vtxSize;
	// 頂点データの読み取り開始位置
	UINT offset = 0;
	
	
	// プリミティブの設定
	// TriangleList ... 頂点を3つずつに区切って
	//					描画
	// 例) [0,1,2] [3,4,5]
	// TriangleStrip ... 参照場所を一つずつずらし
	//					 ながら描画
	// 例) [0,1,2] [1,2,3] [2,3,4]
	pContext->IASetPrimitiveTopology(primitive);

	// インスタンシングあり
	if (m_pInputBuffer != NULL) {
		// 描画に使用するバッファの設定
		ID3D11Buffer* pBuf[2] = { m_pVertexBuffer, m_pInputBuffer };
		UINT stride[2] = { m_vtxSize, m_iptSize };
		UINT offset[2] = { 0, 0 };
		pContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		if (m_idxCount > 0) {
			pContext->IASetIndexBuffer(m_pIndexBuffer, m_idxSize == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
			pContext->DrawIndexedInstanced(m_idxCount, num, 0, 0, 0);
		}
		else {
			pContext->DrawInstanced(m_vtxCount, num, 0, 0);
		}
	}
	// インスタンシングなし
	else {
		// 描画に使用するバッファの設定
		pContext->IASetVertexBuffers(
			0, // GPUへの割り当て先
			1, // 頂点バッファの数
			&m_pVertexBuffer, // 頂点バッファ
			&stride,	// 頂点のサイズ
			&offset		// 読み取り開始位置
		);

		// 描画
		// インデックスバッファの有無で
		// 描画処理が若干異なる
		if (m_idxCount > 0)
		{
			// 描画に使用するインデックス
			// バッファを設定
			pContext->IASetIndexBuffer(
				m_pIndexBuffer,
				// ｲﾝﾃﾞｯｸｽ一つあたりの大きさ
				// 4byte = 32bit
				// 2byte = 16bit
				m_idxSize == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
				0);

			pContext->DrawIndexed(m_idxCount, 0, 0);
		}
		else
		{
			pContext->Draw(m_vtxCount, 0);
		}
	}
}



HRESULT DrawBuffer::Write(void* pVtx) {
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;
	// データコピー
	hr = pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		rsize_t size = m_vtxCount * m_vtxSize;
		memcpy_s(mapResource.pData, size, pVtx, size);
		pContext->Unmap(m_pVertexBuffer, 0);
	}
	return hr;
}

HRESULT DrawBuffer::WriteInstanceng(void* data) {
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;
	// データコピー
	hr = pContext->Map(m_pInputBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		rsize_t size = m_iptCount * m_iptSize;
		memcpy_s(mapResource.pData, size, data, size);
		pContext->Unmap(m_pVertexBuffer, 0);
	}
	return hr;
}






//InstancengBuffer::InstancengBuffer()
//	: m_pInputBuffer(NULL)
//	, m_iptSize(0)
//	, m_iptCount(0)
//{
//}
//InstancengBuffer::~InstancengBuffer() {
//	SAFE_RELEASE(m_pInputBuffer);
//}
//
//HRESULT InstancengBuffer::CreateInputBuffer(UINT size, UINT count) {
//	//// バッファの情報を設定
//	//D3D11_BUFFER_DESC bufDesc = {};
//	//bufDesc.ByteWidth = size * count;
//	//bufDesc.Usage = D3D11_USAGE_DEFAULT;
//	//bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	//// バッファの初期データ
//	//D3D11_SUBRESOURCE_DATA subResource = {};
//
//	// 頂点バッファ インスタンシング用 作成
//	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
//	D3D11_BUFFER_DESC bufDesc = {};
//	HRESULT hr;
//	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufDesc.ByteWidth = sizeof(size) * count;
//	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	bufDesc.MiscFlags = 0;
//	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
//	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pInputBuffer);
//
//	return hr;
//}
//
//
//HRESULT InstancengBuffer::Write(void* data)
//{
//	HRESULT hr;
//	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
//	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
//	D3D11_MAPPED_SUBRESOURCE mapResource;
//
//	// データコピー
//	hr = pContext->Map(m_pInputBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
//	if (SUCCEEDED(hr))
//	{
//		memcpy_s(mapResource.pData, mapResource.RowPitch, data, sizeof(m_iptSize));
//		pContext->Unmap(m_pInputBuffer, 0);
//	}
//	return hr;
//}