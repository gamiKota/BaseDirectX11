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
#include "GameObjectMeshBase.h"
#include "TextureManager.h"
#include "Mesh.h"


#define MAX_DROP 256


class Drop;


/**
 * @struct WaterSurfaceParam
 */
typedef struct {

	// ���ۂɃV�F�[�_�ɗ^������p�����[�^
	float m_pos[2];		//!< UV���W(x, y)
	float m_time;		//!< �o�ߎ���(z)
	float m_influence;	//!< �e����(w)

	// �p�����[�^�ɓ����␳�l
	float m_influenceSub;	//!< �e���͂̌��Z�p�����[�^

} WaterSurfaceParam;


/**
 * @class WaterSurface : inheritance GameObjectMeshBase
 */
class WaterSurface : public GameObjectMeshBase {
public:
	MESH				m_mesh;					//!< ���b�V��
	//WaterSurfaceParam*	m_dropList[MAX_DROP];	//!< ��(�ϒ��͂�����ƌ�����)
	Drop*				m_dropList[MAX_DROP];	//!< ��
	float				m_dropTime;				//!< ����������
	//std::list<Drop*>	m_dropList;	//!< ��

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


	void SetImGuiVal();
};


// EOF