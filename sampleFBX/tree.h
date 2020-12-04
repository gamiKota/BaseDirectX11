//=============================================================================
//
// �؏��� [tree.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include <Windows.h>
#include <DirectXMath.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TREE				(256)	// �؍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct TTree {
	DirectX::XMFLOAT3 pos;		// �ʒu
	DirectX::XMFLOAT4 col;		// �F
	float width;				// ��
	float height;				// ����
	int idxShadow;				// �eID
	bool bUse;					// �g�p���Ă��邩�ǂ���
} TREE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(DirectX::XMFLOAT3 pos, float width, float height, DirectX::XMFLOAT4 col);

TREE *GetTree(void);
