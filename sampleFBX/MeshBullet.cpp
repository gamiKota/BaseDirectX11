//=============================================================================
//
// �r���{�[�h�e [bullet.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "MeshBullet.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "D3DClass.h"
#include "System.h"
//#include "shadow.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/bullet000.png"	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BULLET			100

#define BULLET_SPEED		5.0f
#define BULLET_RADIUS		5.0f

#define MIN_FIELD_X			-320.0f
#define MAX_FIELD_X			320.0f
#define MIN_FIELD_Z			-320.0f
#define MAX_FIELD_Z			320.0f

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct {
	int			stat;		// ���
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT3	vel;		// ���x
} BULLET;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static MESH							g_mesh;					// �\����
static MATERIAL						g_material;				// �}�e���A��

static BULLET						g_bullet[MAX_BULLET];	// �r���{�[�h�e���

// ������
void MeshBullet::Awake()
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;
	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Load Texture Faild", L"MeshBullet", MB_OK);
	}

	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// ���_���̍쐬
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3( BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3( BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;
}

// �I������
void MeshBullet::Uninit()
{
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (pBullet->stat) {
			//ReleaseShadow(pBullet->nShadow);
			//pBullet->nShadow = 0;
			pBullet->stat = 0;
		}
	}
	ReleaseMesh(&g_mesh);
}

// �X�V
void MeshBullet::Update() {
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (!pBullet->stat) continue;
		// �ʒu���X�V
		pBullet->pos.x += pBullet->vel.x;
		pBullet->pos.y += pBullet->vel.y;
		pBullet->pos.z += pBullet->vel.z;
		// �͈̓`�F�b�N
		if (pBullet->pos.x < MIN_FIELD_X ||
			pBullet->pos.x > MAX_FIELD_X ||
			pBullet->pos.z < MIN_FIELD_Z ||
			pBullet->pos.z > MAX_FIELD_Z) {
			pBullet->stat = 0;
			// �ۉe���
			//ReleaseShadow(pBullet->nShadow);
			//pBullet->nShadow = 0;
			continue;
		}

		// �ۉe�ړ�
		//MoveShadow(pBullet->nShadow, pBullet->pos);
	}
}

// �`��
void MeshBullet::Draw()
{
	ID3D11DeviceContext* pDC = D3DClass::GetInstance().GetDeviceContext();
	
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);
	//SetBlendState(BS_ALPHABLEND);

	XMFLOAT4X4& mView = CCamera::Get()->GetView();
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (!pBullet->stat) continue;
		// �r���[�s��̉�]�����̓]�u�s���ݒ�
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pBullet->pos.x;
		g_mesh.mtxWorld._42 = pBullet->pos.y;
		g_mesh.mtxWorld._43 = pBullet->pos.z;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}

	D3DClass::GetInstance().SetBlendState(BS_NONE);
}

// ����
int MeshBullet::Fire(XMFLOAT3 pos, XMFLOAT3 dir)
{
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (pBullet->stat) continue;
		pBullet->pos = pos;
		pBullet->vel.x = dir.x * BULLET_SPEED;
		pBullet->vel.y = dir.y * BULLET_SPEED;
		pBullet->vel.z = dir.z * BULLET_SPEED;
		//pBullet->nShadow = CreateShadow(pos, BULLET_RADIUS);
		pBullet->stat = 1;
		return i;
	}
	return -1;
}


// EOF