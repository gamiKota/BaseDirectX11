//=============================================================================
//
// 数値表示処理 [number.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once


#include <DirectXMath.h>


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_SIZE_X		35.0f	// ポリゴンのサイズ
#define NUMBER_SIZE_Y		50.0f	// ポリゴンのサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitNumber(void);
void UninitNumber(void);

void DrawNumber(DirectX::XMFLOAT2* pPos, unsigned uNumber, int nWidth,
	float fSizeX = NUMBER_SIZE_X, float fSizeY = NUMBER_SIZE_Y);
