/**
 * @file TextureManager.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Singleton.h"
#include <d3d11.h>


/**
 * @enum
 */
enum E_TEXTURE {
	E_TEXTURE_NONE,							// テクスチャなし
	E_TEXTURE_WATER_SURFACE,				// 波
	E_TEXTURE_NUMBER,						// 数字
	E_TEXTURE_TEXT,							// 文字
	E_TEXTURE_TREE,							// 木
	E_TEXTURE_EXPLOSION,					// 爆発エフェクト
	E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI,		// 画面外アイコン(小)
	E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK,	// 画面外アイコン(大)
	E_TEXTURE_ROCK_ICON_INCAMERA_MAIN,		// 画面内メインターゲット
	E_TEXTURE_ROCK_ICON_INCAMERA_SUB,		// 画面内サブターゲット
	E_TEXTURE_ROCK_ICON_TARGET,				// ターゲット表示
	E_TEXTURE_WEAPONS,						// 弾(文字)
	E_TEXTURE_MAX,							// テクスチャ最大値
};


/**
 * @class TextureManager : inheritance Singleton<T>
 */
class TextureManager : public Singleton<TextureManager> {
private:
	ID3D11ShaderResourceView* m_ptexToon;
	
	ID3D11ShaderResourceView* m_pTexture[E_TEXTURE_MAX];
	bool m_isUse[E_TEXTURE_MAX];

public:
	/**
	 * @brief Initialize
	 */
	void Initialize();

	/**
	 * @brief Load
	 */
	void Load(E_TEXTURE texture);

	/**
	 * @brief Release
	 */
	void Release();

	/**
	 * @brief Shutdown
	 */
	void Shutdown();

	ID3D11ShaderResourceView** GetToonTex();

	/**
	 * @brief Get
	 * @param[in] texture
	 * @return ID3D11ShaderResourceView& tex
	 */
	ID3D11ShaderResourceView* Get(E_TEXTURE texture) {
		if (texture <= E_TEXTURE_NONE || texture > E_TEXTURE_MAX)	return nullptr;
		return m_pTexture[texture];
	}
};

// EOF