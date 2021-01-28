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
	ID3D11Buffer* pVertexBuffer;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	ID3D11Buffer* pIndexBuffer;				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	int nNumVertex;							// �����_��	
	int nNumIndex;							// ���C���f�b�N�X��

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
HRESULT InitMesh(void);
void UninitMesh(void);
void DrawMesh(MESH* pMesh, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4X4* matrix);
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh, VERTEX_3D vertexWk[], int indexWk[]);
void ReleaseMesh(MESH* pMesh);
