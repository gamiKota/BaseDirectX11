#pragma once


#include <Windows.h>
#include <d3d11.h>


enum {
	E_SHADER_DEFAULT,
	E_SHADER_LAMBERT,
	E_SHADER_PHONG,
	E_SHADER_MONOCHROME,
	E_SHADER_MAX,
};


class ShaderBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	ShaderBase();

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~ShaderBase();

	/**
	 * @brief �V�F�[�_�̓ǂݍ��݁A�쐬
	 */
	HRESULT Create(const char* FileName);

	/**
	 * @brief �K�������V�F�[�_�����߂�֐�
	 */
	virtual void Bind() = 0;

	/**
	 * @brief �e�N�X�`���̃Z�b�g
	 */
	virtual void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex) {}

protected:

	/**
	 * @brief �V�F�[�_�̍쐬
	 */
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:
	ID3D11ShaderResourceView* m_pTexture;	//!< �e�N�X�`��
};



class ShaderManager
{

};




/*-----------------------------------------------------------------------------------------
�V�F�[�_�݌v����
���܂�Z�ʂ������Ȃ��Ă�����
�V�F�[�_���̂��̂̎�ނŕ�����
VertexShader
GeometryShader
PixelShader
�S���A��

��)
���m�N���V�F�[�_ �� (PixelShader�����K�p)
���f�����C���\�� �� (GeometryShader�����K�p)


-----------------------------------------------------------------------------------------*/



// EOF