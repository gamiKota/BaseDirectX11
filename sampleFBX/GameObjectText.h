#pragma once

#include "GameObjectUI.h"

class Material;

class GameObjectText : GameObject{
private:
	char m_szText[64];		//!< 文字情報
	float m_fontSize[2];	//!< フォントサイズ

	E_TEXTURE	m_texture;	//!< テクスチャ
	E_LAYER		m_layer;	//!< レイヤー

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
	GameObjectText(E_LAYER layer, E_TEXTURE texture = E_TEXTURE_NONE, std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~GameObjectText();

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


	void SetText(const char *fmt, ...);
	void SetFontSize(float size);
	void SetFontSize(float w, float h);
};

// EOF