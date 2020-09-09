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
	E_TEXTURE_NONE,		//!< テクスチャなし
	E_TEXTURE_NUMBER,	//!< 数字
	E_TEXTURE_MAX,		//!< テクスチャ最大値
};


/**
 * @class TextureManager : inheritance Singleton<T>
 */
class TextureManager : public Singleton<TextureManager> {
private:
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
	void Release(E_TEXTURE texture);

	/**
	 * @brief Shutdown
	 */
	void Shutdown();

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