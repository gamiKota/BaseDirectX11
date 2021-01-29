//=============================================================================
//
// ���b�V�����ʏ��� [mesh.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once


#include <DirectXMath.h>
#include "D3DClass.h"
#include "Component.h"


//*****************************************************************************
// �萔��`
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

	DirectX::XMFLOAT4X4 mtxTexture;			// �e�N�X�`�� �}�g���b�N�X
	float3 texPattern;						// �e�N�X�`�����W
	float3 texSize;							// �e�N�X�`���T�C�Y

	bool light = false;						// ���C�g�ݒ�ON/OFF
	bool Zbuff;								// Z�o�b�t�@
	bool isDraw;
} MESH;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawMesh(MESH* pMesh, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4X4* matrix);