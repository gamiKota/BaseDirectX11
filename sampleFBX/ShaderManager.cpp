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

	// �V�F�[�_�̏������L�q���ꂽ�o�C�i���t�@�C����ǂݍ���
	FILE* fp;
	fp = fopen(FileName, "rb");
	if (fp == NULL) { return hr; }

	// �t�@�C���T�C�Y�v�Z
	UINT size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	// �f�[�^�̓ǂݍ���
	fseek(fp, 0, SEEK_SET);
	BYTE* pData = new BYTE[size];
	fread_s(pData, size, size, 1, fp);
	fclose(fp);

	// �V�F�[�_�̍쐬
	hr = MakeShader(pData, size);

	delete[] pData;

	return hr;
}