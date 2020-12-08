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


enum class E_LAYER {
	BACKGROUND	= 0,
	UI			= 5,
	MAX			= 10,
	BILLBOARD	= 11,
};


/**
 * @class GameObjectUI : inheritance GameObject
 */
class GameObjectUI : public GameObject {
public:
	E_TEXTURE	m_texture;
	E_LAYER		m_layer;
	float3		m_color;
	E_SHADER_2D m_shader;

public:
	/**
	 * @brief コンストラクタ
	 */
	GameObjectUI(E_LAYER layer);

	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectUI(E_LAYER layer, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~GameObjectUI();

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