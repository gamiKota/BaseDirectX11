/**
 * @file ShaderManager.cpp
 */


/**
 * @ignore warning
 */
#define _CRT_SECURE_NO_WARNINGS


/**
 * @include
 */
#include "ShaderManager.h"
#include <stdio.h>
#include "System.h"


ShaderBase::ShaderBase()
	: m_pTexture(nullptr)
{
}


ShaderBase::~ShaderBase()
{
}


HRESULT ShaderBase::Create(const char* FileName)
{
	HRESULT hr = E_FAIL;

	// シェーダの処理が記述されたバイナリファイルを読み込む
	FILE* fp;
	fp = fopen(FileName, "rb");
	if (fp == NULL) { return hr; }

	// ファイルサイズ計算
	UINT size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	// データの読み込み
	fseek(fp, 0, SEEK_SET);
	BYTE* pData = new BYTE[size];
	fread_s(pData, size, size, 1, fp);
	fclose(fp);

	// シェーダの作成
	hr = MakeShader(pData, size);

	delete[] pData;

	return hr;
}