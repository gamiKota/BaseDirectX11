#pragma once

#include "GameObjectUI.h"

class Material;

class GameObjectText : GameObject{
private:
	char m_szText[64];		//!< �������
	float m_fontSize[2];	//!< �t�H���g�T�C�Y

	E_TEXTURE	m_texture;	//!< �e�N�X�`��
	E_LAYER		m_layer;	//!< ���C���[

	float3		m_texPattern;	//!< �e�N�X�`�����W
	float3		m_texSize;		//!< �e�N�X�`���T�C�Y
	Material*	m_material;		//!< �}�e���A��

	E_VS		m_vs;	//!< ���_�V�F�[�_
	E_PS		m_ps;	//!< �s�N�Z���V�F�[�_

public:
/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectText(E_LAYER layer, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~GameObjectText();

	/**
	 * @brief ��������
	 */
	virtual void Init();

	/**
	 * @�I������
	 */
	virtual void Uninit();

	/**
	 * @brief �X�V����
	 */
	virtual void Update();

	/**
	 * @brief �ŏI�X�V����
	 */
	virtual void LastUpdate();

	/**
	 * @brief �`�揈��
	 */
	virtual void Draw();


	void SetText(const char *fmt, ...);
	void SetFontSize(float size);
	void SetFontSize(float w, float h);
};

// EOF