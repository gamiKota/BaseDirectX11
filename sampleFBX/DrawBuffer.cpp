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
	//--- �쐬����o�b�t�@�̏��
	D3D11_BUFFER_DESC bufDesc = {};
	// �o�b�t�@�̑傫��
	bufDesc.ByteWidth =
		size * count;
	// �o�b�t�@�̎g�p���@
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	// �o�b�t�@�̗��p��
	bufDesc.BindFlags =
		D3D11_BIND_VERTEX_BUFFER;
	// �������ݎw��
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	//--- �o�b�t�@�̏����l��ݒ�
	D3D11_SUBRESOURCE_DATA
		subResource = {};
	// �������Ɏg���f�[�^�̒u���ꏊ
	// �̎w��
	subResource.pSysMem = pVtx;

	//--- ���_�o�b�t�@�̍쐬
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(
		&bufDesc, &subResource,
		&m_pVertexBuffer);

	//--- ���̂ق��̏���ݒ�
	if (SUCCEEDED(hr))
	{
		m_vtxSize = size;
		m_vtxCount = count;
	}
	return hr;
}

HRESULT DrawBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// �o�b�t�@�̏���ݒ�
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// �o�b�t�@�̏����f�[�^
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// �C���f�b�N�X�o�b�t�@����
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

	// ���_�o�b�t�@ �C���X�^���V���O�p �쐬
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
	// ���_�������̑傫��
	UINT stride = m_vtxSize;
	// ���_�f�[�^�̓ǂݎ��J�n�ʒu
	UINT offset = 0;
	
	
	// �v���~�e�B�u�̐ݒ�
	// TriangleList ... ���_��3���ɋ�؂���
	//					�`��
	// ��) [0,1,2] [3,4,5]
	// TriangleStrip ... �Q�Əꏊ��������炵
	//					 �Ȃ���`��
	// ��) [0,1,2] [1,2,3] [2,3,4]
	pContext->IASetPrimitiveTopology(primitive);

	// �C���X�^���V���O����
	if (m_pInputBuffer != NULL) {
		// �`��Ɏg�p����o�b�t�@�̐ݒ�
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
	// �C���X�^���V���O�Ȃ�
	else {
		// �`��Ɏg�p����o�b�t�@�̐ݒ�
		pContext->IASetVertexBuffers(
			0, // GPU�ւ̊��蓖�Đ�
			1, // ���_�o�b�t�@�̐�
			&m_pVertexBuffer, // ���_�o�b�t�@
			&stride,	// ���_�̃T�C�Y
			&offset		// �ǂݎ��J�n�ʒu
		);

		// �`��
		// �C���f�b�N�X�o�b�t�@�̗L����
		// �`�揈�����኱�قȂ�
		if (m_idxCount > 0)
		{
			// �`��Ɏg�p����C���f�b�N�X
			// �o�b�t�@��ݒ�
			pContext->IASetIndexBuffer(
				m_pIndexBuffer,
				// ���ޯ���������̑傫��
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
	// �f�[�^�R�s�[
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
	// �f�[�^�R�s�[
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
//	//// �o�b�t�@�̏���ݒ�
//	//D3D11_BUFFER_DESC bufDesc = {};
//	//bufDesc.ByteWidth = size * count;
//	//bufDesc.Usage = D3D11_USAGE_DEFAULT;
//	//bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	//// �o�b�t�@�̏����f�[�^
//	//D3D11_SUBRESOURCE_DATA subResource = {};
//
//	// ���_�o�b�t�@ �C���X�^���V���O�p �쐬
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
//	// �f�[�^�R�s�[
//	hr = pContext->Map(m_pInputBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
//	if (SUCCEEDED(hr))
//	{
//		memcpy_s(mapResource.pData, mapResource.RowPitch, data, sizeof(m_iptSize));
//		pContext->Unmap(m_pInputBuffer, 0);
//	}
//	return hr;
//}