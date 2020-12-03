#pragma once

#include <Windows.h>
#include <d3d11.h>


/**
 * @brief �V�F�[�_�̒萔�o�b�t�@�Ƀf�[�^��n��
 */
class ShaderBuffer
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	ShaderBuffer();

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~ShaderBuffer();

	/**
	 * @brief ����
	 * @return HRESULT
	 */
	HRESULT Create(UINT size);
	void UpdateSource(void* pData);
	void BindVS(UINT slot);
	void BindPS(UINT slot);
	void BindGS(UINT slot);

private:
	ID3D11Buffer* m_pBuffer;
};