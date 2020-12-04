//=============================================================================
//
// �؏��� [tree.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tree.h"
#include "camera.h"
//#include "shadow.h"
#include "light.h"
#include "Texture.h"
#include "Mesh.h"
#include "D3DClass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TREE			L"data/Texture/tree000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TREE_WIDTH				(50.0f)							// �e�̔��a��
#define	TREE_HEIGHT				(80.0f)							// �e�̔��a��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTree(ID3D11Device* pDevice);
void SetVertexTree(int idxTree, float width, float height);
void SetColorTree(int idxTree, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_meshTree;						// ���b�V�����
static MATERIAL			g_material;
static TREE				g_aTree[MAX_TREE];				// �؃��[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTree(void)
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	// ���_���̍쐬
	MakeVertexTree(pDevice);

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
						  TEXTURE_TREE,				// �t�@�C���̖��O
						  &g_meshTree.pTexture);	// �ǂݍ��ރ������[
	XMStoreFloat4x4(&g_meshTree.mtxTexture, XMMatrixIdentity());

	g_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Power = 0.0f;
	g_meshTree.pMaterial = &g_material;

	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		g_aTree[cntTree].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aTree[cntTree].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTree[cntTree].width = TREE_WIDTH;
		g_aTree[cntTree].height = TREE_HEIGHT;
		g_aTree[cntTree].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTree(void)
{
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			//DeleteShadow(g_aTree[cntTree].idxShadow);
			g_aTree[cntTree].idxShadow = -1;
			g_aTree[cntTree].bUse = false;
		}
	}
	// ���b�V���̊J��
	ReleaseMesh(&g_meshTree);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTree(void)
{
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			// �e�̈ʒu�ݒ�
			//SetPositionShadow(g_aTree[cntTree].idxShadow, XMFLOAT3(g_aTree[cntTree].pos.x, 0.1f, g_aTree[cntTree].pos.z));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTree(void)
{
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ���C�e�B���O����
	SetLightEnable(false);

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetView();
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshTree.mtxWorld, mtxWorld);

			g_meshTree.mtxWorld._11 = mtxView._11;
			g_meshTree.mtxWorld._12 = mtxView._21;
			g_meshTree.mtxWorld._13 = mtxView._31;
			g_meshTree.mtxWorld._21 = mtxView._12;
			g_meshTree.mtxWorld._22 = mtxView._22;
			g_meshTree.mtxWorld._23 = mtxView._32;
			g_meshTree.mtxWorld._31 = mtxView._13;
			g_meshTree.mtxWorld._32 = mtxView._23;
			g_meshTree.mtxWorld._33 = mtxView._33;
			mtxWorld = XMLoadFloat4x4(&g_meshTree.mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(g_aTree[cntTree].width, g_aTree[cntTree].height, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aTree[cntTree].pos.x, g_aTree[cntTree].pos.y, g_aTree[cntTree].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_meshTree.mtxWorld, mtxWorld);

			// �F�̐ݒ�
			g_material.Diffuse = g_aTree[cntTree].col;

			// �|���S���̕`��
			DrawMesh(pDeviceContext, &g_meshTree);
		}
	}

	// ���C�e�B���O�L��
	SetLightEnable(true);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexTree(ID3D11Device* pDevice)
{
	// �ꎞ�I�Ȓ��_�z��𐶐�
	g_meshTree.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshTree.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2.0f, 1.0f, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2.0f, 1.0f, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	g_meshTree.nNumIndex = 4;
	int* pIndexWk = new int[g_meshTree.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	HRESULT hr = MakeMeshVertex(pDevice, &g_meshTree, pVertexWk, pIndexWk);

	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTree(int idxTree, float width, float height)
{
	if (idxTree >= 0 && idxTree < MAX_TREE) {
		g_aTree[idxTree].width = width;
		g_aTree[idxTree].height = height;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorTree(int idxTree, XMFLOAT4 col)
{
	if (idxTree >= 0 && idxTree < MAX_TREE) {
		g_aTree[idxTree].col = col;
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetTree(XMFLOAT3 pos, float width, float height, XMFLOAT4 col)
{
	int idxTree = -1;

	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (!g_aTree[cntTree].bUse) {
			g_aTree[cntTree].pos = pos;
			g_aTree[cntTree].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexTree(cntTree, width, height);

			// ���_�J���[�̐ݒ�
			SetColorTree(cntTree, col);

			// �e�̐ݒ�
			//g_aTree[cntTree].idxShadow = SetShadow(g_aTree[cntTree].pos, g_aTree[cntTree].width, g_aTree[cntTree].width);

			idxTree = cntTree;
			break;
		}
	}

	return idxTree;
}

//=============================================================================
// �؂��擾
//=============================================================================
TREE *GetTree(void)
{
	return &g_aTree[0];
}
