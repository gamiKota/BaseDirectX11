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
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = size;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// バッファの生成
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);

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


// EOF