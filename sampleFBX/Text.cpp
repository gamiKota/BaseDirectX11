#include "Text.h"
#include "Graphics.h"
#include "polygon.h"
#include <Windows.h>
#include <stdio.h>
#include "TextureManager.h"
#include "System.h"

#define FONT_WIDTH			16
#define FONT_HEIGHT			16

void Text::Set(const char *fmt, ...) {
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

void Text::Bind() {
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	XMFLOAT2 vPos(SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f - FONT_HEIGHT * 0.5f);
	SetPolygonTexture(TextureManager::GetInstance().Get(E_TEXTURE_TEXT));
	SetPolygonFrameSize(8.0f / 128.0f, 8.0f / 128.0f);
	SetPolygonSize(FONT_WIDTH, FONT_HEIGHT);
	SetPolygonAngle(0.f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.f);

	//SetPolygonAlpha(0.5f);

	for (char* pChr = &m_szText[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f;
			vPos.y -= FONT_HEIGHT;
			continue;
		}
		SetPolygonPos(vPos.x, vPos.y);
		int nChr = (BYTE)*pChr;
		//if (g_bHiragana &&
		//	(nChr >= 0xa6 && nChr <= 0xaf || nChr >= 0xb1 && nChr <= 0xdd))
		//	nChr ^= 0x20;
		SetPolygonUV((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f);
		UpdatePolygon();
		DrawPolygon(pDeviceContext);
		vPos.x += FONT_WIDTH;
	}
	// テクスチャ設定を元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}

// EOF