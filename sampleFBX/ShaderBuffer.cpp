#include "ShaderBuffer.h"
#include "D3DClass.h"
#include "System.h"



ShaderBuffer::ShaderBuffer() : m_pBuffer(NULL) {

}


ShaderBuffer::~ShaderBuffer() {
	SAFE_RELEASE(m_pBuffer);
}


HRESULT ShaderBuffer::Create(UINT size) {
	// 作成するバッファの情報を設定
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));	// HMY
	bd.ByteWidth = size;
	//bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// HMY
	bd.Usage = D3D11_USAGE_DYNAMIC;	// HMY
	// バッファの生成
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);

	//// 頂点バッファ インスタンシング用 作成
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.ByteWidth = sizeof(XMMATRIX) * 100;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//bd.MiscFlags = 0;
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	//hr = pDevice->CreateBuffer(&bd, nullptr, &m_pBuffer);

	return hr;
}


void ShaderBuffer::UpdateSource(void* pData) {
	// 定数バッファの中身を更新
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->UpdateSubresource(m_pBuffer, 0, NULL, pData, 0, 0);
}


void ShaderBuffer::BindVS(UINT slot) {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->VSSetConstantBuffers(slot, 1, &m_pBuffer);
}


void ShaderBuffer::BindPS(UINT slot) {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->PSSetConstantBuffers(slot, 1, &m_pBuffer);
}


void ShaderBuffer::BindGS(UINT slot) {
	ID3D11DeviceContext* pContext = D3DClass::GetInstance().GetDeviceContext();
	pContext->GSSetConstantBuffers(slot, 1, &m_pBuffer);
}




void ShaderBufferManager::Initialize() {
	m_registerMap.clear();
	m_registerMap["World"] = 0;
	m_registerMap["ViewPro"] = 1;
}


// EOF