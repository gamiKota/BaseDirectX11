//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include <Windows.h>

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);

void StartDebugProc(bool hiragana = false);
void PrintDebugProc(const char *fmt, ...);
bool SetHiragana(bool hiragana = true);
