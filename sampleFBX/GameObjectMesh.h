/**
 * @file GameObjectMesh.h
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


enum class E_MESH_TYPE {
	NORMAL = 0,
	BILLBORAD,
};


/**
 * @class GameObjectMesh : inheritance GameObject
 */
class GameObjectMesh : public GameObject {
public:
	E_TEXTURE		m_texture;	//!< �e�N�X�`��
	E_MESH_TYPE		m_type;		//!< �\�����@
	MESH			m_mesh;		//!< ���b�V��
	Material*		m_material;	//!< �}�e���A��

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	GameObjectMesh(E_MESH_TYPE mesh);

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectMesh(E_MESH_TYPE mesh, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~GameObjectMesh();

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