//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "Data.h"
#include "D3DClass.h"


enum class E_SHADER_2D {
	_NORMAL,	// 通常
	_COLOR,		// 色直塗
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPolygon(ID3D11Device* pDevice);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(ID3D11DeviceContext* pDeviceContext);
void SetPolygonTexture(ID3D11ShaderResourceView* pTexture);
void SetPolygonPos(float fX, float fY);
void SetPolygonSize(float fScaleX, float fScaleY);
void SetPolygonAngle(float fAngle);
void SetPolygonAngle(float3 angle);
void SetPolygonUV(float fU, float fV);
void SetPolygonFrameSize(float fWidth, float fHeight);
void SetPolygonColor(float fRed, float fGreen, float fBlue);
void SetPolygonAlpha(float fAlpha);
void SetPolygonShader(E_SHADER_2D shader);