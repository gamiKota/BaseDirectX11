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

void DrawBuffer::Draw(
	D3D11_PRIMITIVE_TOPOLOGY primitive)
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
	pContext->
		IASetPrimitiveTopology(primitive);
	
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

		// ���ޯ���ޯ̧���g�p���ĕ`��
		pContext->DrawIndexed(
			m_idxCount, 0, 0);
	}
	else
	{
		// ���_�o�b�t�@�݂̂ŕ`��
		pContext->Draw(m_vtxCount, 0);
	}

}


HRESULT DrawBuffer::Write(void* pVtx)
{
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