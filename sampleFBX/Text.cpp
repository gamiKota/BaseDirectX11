#include "Text.h"
#include "Graphics.h"
#include "polygon.h"
#include <Windows.h>
#include <stdio.h>
#include "TextureManager.h"
#include "Material.h"
#include "ShaderManager.h"
#include "Geometory.h"
#include "System.h"

static XMFLOAT4X4					g_mWorld;				// ワールド変換行列
static XMFLOAT4X4					g_mTex;					// テクスチャ変換行列

Text::Text() {
	m_szText[0] = '\0';
	m_fontSize[0] = 16;
	m_fontSize[1] = 16;

	m_material = new Material;
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値はテクスチャrgbはモデル自体の色
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値を０にすると真っ白 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 値を小さくするとモデルが薄くなる
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// 光沢
}

Text::~Text() {
	delete m_material;
}

void Text::SetText(const char *fmt, ...) {
	va_list list;
	char aBuf[256];

	m_szText[0] = '\0';

	va_start(list, fmt);
	_vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// 連結
	if ((strlen(m_szText) + strlen(aBuf)) < sizeof(m_szText) - 1) {
		strcat_s(m_szText, sizeof(m_szText), aBuf);
	}
}

void Text::SetFontSize(float size) {
	SetFontSize(size, size);
}

void Text::SetFontSize(float w, float h) {
	m_fontSize[0] = w;
	m_fontSize[1] = h;
}

void Text::Bind() {

	ShaderManager* shader = &ShaderManager::GetInstance();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	// マテリアル
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// テクスチャのセット
	shader->SetTexturePS(TextureManager::GetInstance().Get(E_TEXTURE_TEXT));


	XMFLOAT2 vPos(SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f,
		SCREEN_HEIGHT * 0.5f - m_fontSize[1] * 0.5f);


	for (char* pChr = &m_szText[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f;
			vPos.y -= m_fontSize[1];
			continue;
		}
		int nChr = (BYTE)*pChr;
		
		// ワールド行列
		XMMATRIX mWorld = XMMatrixScaling(m_fontSize[0], m_fontSize[1], 0.f);
		mWorld *= XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f);
		mWorld *= XMMatrixTranslation(vPos.x, vPos.y, 0.f);
		XMStoreFloat4x4(&g_mWorld, mWorld);
		// テクスチャ行列
		mWorld = XMMatrixScaling(8.0f / 128.0f, 8.0f / 128.0f, 1.0f);
		mWorld *= XMMatrixTranslation((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f, 0.0f);
		XMStoreFloat4x4(&g_mTex, mWorld);
		// 定数バッファへ格納
		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
		world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
		shader->UpdateBuffer("MainWorld", &world);
		// ポリゴンの描画
		DrawPolygon();

		vPos.x += m_fontSize[0];
	}
}



/*
if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f;
			vPos.y -= m_fontSize[1];
			continue;
		}

		int nChr = (BYTE)*pChr;

		// ワールド行列
		XMMATRIX mWorld = XMMatrixScaling(m_fontSize[0], m_fontSize[1], 0.f);
		mWorld *= XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f);
		mWorld *= XMMatrixTranslation(vPos.x, vPos.y, 0.f);
		XMStoreFloat4x4(&g_mWorld, mWorld);
		// テクスチャ行列
		mWorld = XMMatrixScaling(8.0f / 128.0f, 8.0f / 128.0f, 1.0f);
		mWorld *= XMMatrixTranslation((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f, 0.0f);
		XMStoreFloat4x4(&g_mTex, mWorld);

		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
		world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
		shader->UpdateBuffer("MainWorld", &world);

		DrawPolygon(pDeviceContext);

		vPos.x += m_fontSize[0];
*/


// EOF