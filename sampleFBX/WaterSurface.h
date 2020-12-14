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
	MESH			m_mesh;		//!< メッシュ
	Material*		m_material;	//!< マテリアル

public:
	/**
	 * @brief コンストラクタ
	 */
	WaterSurface();

	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	WaterSurface(std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~WaterSurface();

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