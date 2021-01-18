/**
 * @file ShaderBuffer
 * @brief シェーダのバッファの定義
 */


/**
 * @include
 */
#include "ShaderBuffer.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Light.h"
#include "System.h"



ShaderBuffer::ShaderBuffer() : m_pBuffer(NULL) {

}


ShaderBuffer::~ShaderBuffer() {
	SAFE_RELEASE(m_pBuffer);
}


HRESULT ShaderBuffer::Create(UINT size, bool f) {
	HRESULT hr;
	if (f) {
		// 作成するバッファの情報を設定
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));	// HMY
		bd.ByteWidth = size;
		//bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// HMY
		bd.Usage = D3D11_USAGE_DYNAMIC;	// HMY
		// バッファの生成
		ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
		hr = pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);
	}
	else {
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		// バッファの生成
		ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
		hr = pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);
	}

	//// 頂点バッファ インスタンシング用 作成
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.ByteWidth = sizeof(XMMATRIX) * 100;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//bd.MiscFlags = 0;
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	//hr = pDevice->CreateBuffer(&bd, nullptr, &m_pBuffer);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "", "", MB_OK);
	}

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
	m_bufferMap.clear();
	this->Create("MainCamera", sizeof(SHADER_CAMERA), 0);
	this->Create("MainLight", sizeof(SHADER_LIGHT), 1);
	//m_bufferMap["MainCamera"]	= new ShaderBuffer();
	//m_bufferMap["MainLight"]	= new ShaderBuffer();
	//m_bufferMap["World"]		= new ShaderBuffer();
	//m_bufferMap["Material"]		= new ShaderBuffer();
	//m_bufferMap["Bone"]			= new ShaderBuffer();
}

void ShaderBufferManager::Terminate() {
	//delete m_bufferMap["Bone"];
	//delete m_bufferMap["Material"];
	//delete m_bufferMap["World"];
	delete m_bufferMap["MainLight"];
	delete m_bufferMap["MainCamera"];
	m_bufferMap.clear();
}

void ShaderBufferManager::Update(std::string bufName, void* pData) {
	m_bufferMap[bufName]->UpdateSource(pData);
}

void ShaderBufferManager::Bind(std::string bufName) {
	m_bufferMap[bufName]->BindPS(m_registerMap[bufName]);
	m_bufferMap[bufName]->BindVS(m_registerMap[bufName]);
	m_bufferMap[bufName]->BindGS(m_registerMap[bufName]);
}

void ShaderBufferManager::Create(std::string bufName, UINT size, int regNum) {
	m_bufferMap[bufName] = new ShaderBuffer;
	m_bufferMap[bufName]->Create(size, false);
	m_registerMap[bufName] = regNum;
}


// EOF