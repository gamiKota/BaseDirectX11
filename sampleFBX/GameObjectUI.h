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
#include "GameObject.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include <string>


enum class E_LAYER {
	BACKGROUND	= 0,
	UI			= 5,
	MAX			= 10,
};


class Material;
class Text;


/**
 * @class GameObjectUI : inheritance GameObject
 */
class GameObjectUI : public GameObject {
public:
	E_TEXTURE	m_texture;	//!< テクスチャ
	E_LAYER		m_layer;	//!< レイヤー
	Text*		m_text;		//!< テキスト

	float3		m_texPattern;	//!< テクスチャ座標
	float3		m_texSize;		//!< テクスチャサイズ
	Material*	m_material;		//!< マテリアル

	E_VS		m_vs;	//!< 頂点シェーダ
	E_PS		m_ps;	//!< ピクセルシェーダ

public:
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


bool GameObjectUISort(GameObject* obj1, GameObject* obj2);


// EOF