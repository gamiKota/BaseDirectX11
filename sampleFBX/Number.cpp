//=============================================================================
//
// 数値表示処理 [number.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Number.h"
#include "polygon.h"
#include "Texture.h"
#include "D3DClass.h"
#include "System.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_TEXTURENAME	L"data/texture/number000.png"	// 読み込むテクスチャファイル名

#define NUMBER_FRAME_X		10	// 横フレーム数
#define NUMBER_FRAME_Y		1	// 縦フレーム数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;			// テクスチャへのポインタ

//=============================================================================
// 数値表示の初期化処理
//=============================================================================
void InitNumber(void)
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, NUMBER_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) {
		MessageBoxA(System::GetInstance().GetWnd(), "error\nload texture", "Number.cpp", MB_OK | MB_ICONERROR | MB_TOPMOST);
	}
}

//=============================================================================
// 数値表示の終了処理
//=============================================================================
void UninitNumber(void)
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// 数値表示
//=============================================================================
void DrawNumber(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
	float fSizeX, float fSizeY)
{
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	SetPolygonTexture(g_pTexture);
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonFrameSize(1.0f / NUMBER_FRAME_X, 1.0f / NUMBER_FRAME_Y);
	float fX = pPos->x + (nWidth - 0.5f) * fSizeX;
	float fY = pPos->y - fSizeY * 0.5f;
	for (int i = 0; i < nWidth; ++i) {
		SetPolygonPos(fX, fY);
		unsigned n = uNumber % 10;
		uNumber /= 10;
		int u = n % NUMBER_FRAME_X;
		int v = n / NUMBER_FRAME_X;
		SetPolygonUV(u / (float)NUMBER_FRAME_X, v / (float)NUMBER_FRAME_Y);
		DrawPolygon(pDeviceContext);
		fX -= fSizeX;
	}
	// テクスチャ サイズ、位置を元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}
