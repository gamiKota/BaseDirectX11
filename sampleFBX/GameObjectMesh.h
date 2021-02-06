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
#include "GameObjectMeshBase.h"
#include "TextureManager.h"
#include "Mesh.h"


enum class E_MESH_TYPE {
	NORMAL = 0,
	BILLBORAD,
};

class Material;

/**
 * @class GameObjectMesh : inheritance GameObjectMeshBase
 */
class GameObjectMesh : public GameObjectMeshBase {
public:
	E_TEXTURE		m_texture;	//!< テクスチャ
	E_MESH_TYPE		m_type;		//!< 表示方法
	MESH			m_mesh;		//!< メッシュ
	Material*		m_material;	//!< マテリアル

public:
	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectMesh(E_MESH_TYPE mesh, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectMesh", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~GameObjectMesh();

	/**
	 * @brief 初期処理
	 */
	virtual void Init();

	/**
	 * @終了処理
	 */
	virtual void Uninit();

	/**
	 * @brief 更新処理
	 */
	virtual void Update();

	/**
	 * @brief 最終更新処理
	 */
	virtual void LastUpdate();

	/**
	 * @brief 描画処理
	 */
	virtual void Draw();
};


// EOF