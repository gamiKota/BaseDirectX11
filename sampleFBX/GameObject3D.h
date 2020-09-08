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


/**
 * @class GameObject : inheritance Object
 */
class GameObject3D : public GameObject {
public:
	E_MODEL m_model;	//!< モデル(いつかコンポーネント化)

public:
	/**
	 * @brief コンストラクタ
	 */
	GameObject3D(E_MODEL m_model = E_MODEL_NONE);

	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObject3D(E_MODEL m_model = E_MODEL_NONE, std::string name = "GameObject3D", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~GameObject3D();

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