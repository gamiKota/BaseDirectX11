/**
 * @file GameObjectUI.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "polygon.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "ShaderManager.h"


enum class E_LAYER {
	BACKGROUND	= 0,
	UI			= 5,
	MAX			= 10,
};


/**
 * @class GameObjectUI : inheritance GameObject
 */
class GameObjectUI : public GameObject {
public:
	E_TEXTURE	m_texture;
	E_LAYER		m_layer;
	float3		m_color;
	float		m_alpha;

	float3 m_texPattern;						// �e�N�X�`�����W
	float3 m_texSize;							// �e�N�X�`���T�C�Y

	E_VS		m_vs;
	E_PS		m_ps;

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	GameObjectUI(E_LAYER layer);

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectUI(E_LAYER layer, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~GameObjectUI();

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
};


bool GameObjectUISort(GameObject* obj1, GameObject* obj2);


// EOF