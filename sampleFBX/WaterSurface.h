/**
 * @file WaterSurface.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "GameObject.h"
#include "TextureManager.h"
#include "Mesh.h"


/**
 * @class WaterSurface : inheritance GameObject
 */
class WaterSurface : public GameObject {
public:
	MESH			m_mesh;		//!< ���b�V��
	Material*		m_material;	//!< �}�e���A��

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	WaterSurface();

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	WaterSurface(std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~WaterSurface();

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


// EOF