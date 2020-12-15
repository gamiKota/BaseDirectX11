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
	// �V�F�[�_�Ƀe�N�X�`����n��
	pContext->VSSetShaderResources(0, texNum, ppTex);
}


HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr = E_FAIL;

	// ���_�V�F�[�_���o�b�t�@�̃f�[�^�\�����ǂ̂悤�ɂȂ��Ă��邩��
	// �m�邽�߂ɁA���_���C�A�E�g�̃f�[�^���쐬����
	// �@���_�t�H�[�}�b�g�̒�`�A����
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_INDEX",  0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// �Apos-color-uv-normal
	static const D3D11_INPUT_ELEMENT_DESC PosColorUvNormal[] = {
		{
			// �Z�}���e�B�N�X
			// ���_�̎g�p���@��\��
			"POSITION", 0,
			// �t�H�[�}�b�g
			DXGI_FORMAT_R32G32B32_FLOAT,
			// �f�[�^�̕Ԃ��ʒu�ƃI�t�Z�b�g�ʒu
			0, 0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},  {
			"COLOR", 0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			// ���O�̃f�[�^�̂������w��(����Ƀo�C�g�����v�Z���Ă����)
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}, {
			"TEXCOORD", 0,
			DXGI_FORMAT_R32G32_FLOAT,
			// ���O�̃f�[�^�̂������w��(����Ƀo�C�g�����v�Z���Ă����)
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}, {
			"NORMAL", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// �Apos-color-uv-normal
	static const D3D11_INPUT_ELEMENT_DESC PosNormalColorUV[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// �Bpos-uv-normal
	static const D3D11_INPUT_ELEMENT_DESC PosUvNormal[] = {
		{
			// �Z�}���e�B�N�X
			// ���_�̎g�p���@��\��
			"POSITION", 0,
			// �t�H�[�}�b�g
			DXGI_FORMAT_R32G32B32_FLOAT,
			// �f�[�^�̕Ԃ��ʒu�ƃI�t�Z�b�g�ʒu
			0, 0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}, {
			"TEXCOORD", 0,
			DXGI_FORMAT_R32G32_FLOAT,
			// ���O�̃f�[�^�̂������w��(����Ƀo�C�g�����v�Z���Ă����)
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}, {
			"NORMAL", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// �Cpos-normal
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

	// ���_���C�A�E�g�쐬
	const D3D11_INPUT_ELEMENT_DESC* pDesc[] = { layout, PosColorUvNormal, PosNormalColorUV ,PosUvNormal, PosNormal };
	int dataNum[] = { _countof(layout), _countof(PosColorUvNormal), _countof(PosNormalColorUV) , _countof(PosUvNormal), _countof(PosNormal) };

	// CPU-GPU�ԂŃf�[�^���s���������邽��
	ID3D11Device *pDevice = D3DClass::GetInstance().GetDevice();
	hr = pDevice->CreateInputLayout(pDesc[m_layout], dataNum[m_layout], pData, size, &m_pInputLayout);
	if (FAILED(hr)) { return hr; }

	// ���_�V�F�[�_�̍쐬
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVertexShader);
	if (FAILED(hr)) { return hr; }

	return hr;
}

// EOF