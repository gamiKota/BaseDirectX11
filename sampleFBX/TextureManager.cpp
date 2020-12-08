/**
 * @TextureManager.cpp
 */


/**
 * @include
 */
#include "TextureManager.h"
#include "Texture.h"
#include "D3DClass.h"
#include "System.h"


/**
 * @const
 */
static const char* name[E_TEXTURE_MAX] = {
	"tetuxre_none",
	"data/texture/number000.png",		// 数字
	"data/texture/tree000.png",			// 木
	"data/texture/explosion000.png",	// 爆発エフェクト
	"data/texture/OutCameraMin.tga",	// 画面外アイコン(小)
	"data/texture/arrow001.tga",		// 画面外アイコン(大)
	"data/texture/LockMain.tga",		// 画面内メインターゲット
	"data/texture/LockSub.tga",			// 画面内サブターゲット
};


void TextureManager::Initialize() {
	for (auto tex : m_pTexture) {
		SAFE_RELEASE(tex);
	}
	for (auto use : m_isUse) {
		use = false;
	}
}


void TextureManager::Load(E_TEXTURE texture) {
	if (texture <= E_TEXTURE_NONE || texture > E_TEXTURE_MAX)	return;
	if (!m_isUse[texture]) {	// テクスチャの読み込み
		if (FAILED(CreateTextureFromFile(D3DClass::GetInstance().GetDevice(), name[texture], &m_pTexture[texture]))) {
			MessageBoxA(System::GetInstance().GetWnd(), "error load texture", "TextureManager.cpp", MB_OK | MB_ICONERROR | MB_TOPMOST);
		}
		m_isUse[texture] = true;
	}
}


void TextureManager::Release() {
	for (int i = 0; i < E_TEXTURE_MAX; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
		m_isUse[i] = false;
	}
}


void TextureManager::Shutdown() {
	for (auto tex : m_pTexture) {
		SAFE_RELEASE(tex);
	}
}


// EOF