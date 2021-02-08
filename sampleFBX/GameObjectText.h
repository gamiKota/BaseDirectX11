#pragma once

#include "GameObjectUI.h"

class Material;

class GameObjectText : public GameObjectUI {
private:
	char m_szText[64];		//!< �������
	float m_fontSize[2];	//!< �t�H���g�T�C�Y

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