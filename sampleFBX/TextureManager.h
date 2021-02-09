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
	E_TEXTURE_NONE,							// �e�N�X�`���Ȃ�
	E_TEXTURE_WATER_SURFACE,				// �g
	E_TEXTURE_NUMBER,						// ����
	E_TEXTURE_TEXT,							// ����
	E_TEXTURE_TREE,							// ��
	E_TEXTURE_EXPLOSION,					// �����G�t�F�N�g
	E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI,		// ��ʊO�A�C�R��(��)
	E_TEXTURE_ROCK_ICON_OUTCAMERA_ATTACK,	// ��ʊO�A�C�R��(��)
	E_TEXTURE_ROCK_ICON_INCAMERA_MAIN,		// ��ʓ����C���^�[�Q�b�g
	E_TEXTURE_ROCK_ICON_INCAMERA_SUB,		// ��ʓ��T�u�^�[�Q�b�g
	E_TEXTURE_ROCK_ICON_TARGET,				// �^�[�Q�b�g�\��
	E_TEXTURE_WEAPONS,						// �e(����)
	E_TEXTURE_MAX,							// �e�N�X�`���ő�l
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