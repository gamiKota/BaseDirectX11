//=============================================================================
//
// 木処理 [tree.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include <Windows.h>
#include <DirectXMath.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TREE				(256)	// 木最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct TTree {
	DirectX::XMFLOAT3 pos;		// 位置
	DirectX::XMFLOAT4 col;		// 色
	float width;				// 幅
	float height;				// 高さ
	int idxShadow;				// 影ID
	bool bUse;					// 使用しているかどうか
} TREE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(DirectX::XMFLOAT3 pos, float width, float height, DirectX::XMFLOAT4 col);

TREE *GetTree(void);
