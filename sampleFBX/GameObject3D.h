/**
 * @file GameObject3D.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "GameObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "FbxModel.h"


/**
 * @class GameObject3D : inheritance GameObject
 */
class GameObject3D : public GameObject {
public:
	E_MODEL			m_model;		//!< ���f��(�����R���|�[�l���g��)
	E_SHADER		m_shader;		//!< �V�F�[�_
	TFbxMaterial	m_material;		//!< �}�e���A��
	bool			m_isLight;		//!< ���C�g�ݒ�

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	GameObject3D();

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObject3D(E_MODEL m_model = E_MODEL_NONE, std::string name = "GameObject3D", std::string tag = "none");

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~GameObject3D();

	/**
	 * @brief ��������
	 */
	virtual void Init();

	/**
	 * @brief �I������
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