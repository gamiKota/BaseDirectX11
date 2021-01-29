//=============================================================================
//
// メッシュ共通処理 [mesh.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once


#include <DirectXMath.h>
#include "D3DClass.h"
#include "Component.h"


//*****************************************************************************
// 定数定義
//*****************************************************************************
enum ePrimitiveType {
	PT_UNDEFINED = 0,
	PT_POINT,
	PT_LINE,
	PT_LINESTRIP,
	PT_TRIANGLE,
	PT_TRIANGLESTRIP,

	MAX_PRIMITIVETYPE
};



typedef struct {

	DirectX::XMFLOAT4X4 mtxTexture;			// テクスチャ マトリックス
	float3 texPattern;						// テクスチャ座標
	float3 texSize;							// テクスチャサイズ

	bool light = false;						// ライト設定ON/OFF
	bool Zbuff;								// Zバッファ
	bool isDraw;
} MESH;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawMesh(MESH* pMesh, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4X4* matrix);