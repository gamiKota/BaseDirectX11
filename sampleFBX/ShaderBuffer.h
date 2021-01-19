#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <map>
#include "Singleton.h"


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
	HRESULT Create(UINT size, bool f = true);
	void UpdateSource(void* pData);
	void BindVS(UINT slot);
	void BindPS(UINT slot);
	void BindGS(UINT slot);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }

private:
	ID3D11Buffer* m_pBuffer;
};


// EOF