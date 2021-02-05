//=============================================================================
//
// �|���S������ [polygon.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "polygon.h"
#include "Shader.h"
#include "Graphics.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "Geometory.h"
#include "System.h"



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexPolygon(ID3D11Device* pDevice);
static void SetVertexPolygon(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D					g_vertexWk[NUM_VERTEX];	// ���_���i�[���[�N

static XMFLOAT3						g_posPolygon;			// �|���S���̈ړ���
static XMFLOAT3						g_rotPolygon;			// �|���S���̉�]��
static XMFLOAT3						g_sizPolygon;			// �|���S���̃T�C�Y
static XMFLOAT4						g_colPolygon;			// �|���S���̒��_�J���[
static bool							g_bInvalidate;			// ���_�f�[�^�X�V�t���O

static XMFLOAT2						g_posTexFrame;			// UV���W
static XMFLOAT2						g_sizTexFrame;			// �e�N�X�`�����o�T�C�Y

static XMFLOAT4X4					g_mWorld;				// ���[���h�ϊ��s��
static XMFLOAT4X4					g_mTex;					// �e�N�X�`���ϊ��s��


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygon(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;

	// �ϊ��s�񏉊���
	XMStoreFloat4x4(&g_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mTex, XMMatrixIdentity());
	g_mTex._44 = 0.0f;

	g_posPolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotPolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizPolygon = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_colPolygon = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_bInvalidate = false;

	g_posTexFrame = XMFLOAT2(0.0f, 0.0f);
	g_sizTexFrame = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPolygon(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePolygon(void)
{
	// (�������Ȃ�)
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPolygon(ID3D11DeviceContext* pDeviceContext)
{
	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(g_sizPolygon.x, g_sizPolygon.y, g_sizPolygon.z);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotPolygon.x),
		XMConvertToRadians(g_rotPolygon.y), XMConvertToRadians(g_rotPolygon.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&g_mWorld, mWorld);

	if (g_pTexture) {
		// �g�k
		mWorld = XMMatrixScaling(g_sizTexFrame.x, g_sizTexFrame.y, 1.0f);
		// �ړ�
		mWorld *= XMMatrixTranslation(g_posTexFrame.x, g_posTexFrame.y, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&g_mTex, mWorld);
	} else {
		// �e�N�X�`������
		g_mTex._44 = 0.0f;
	}

	ShaderManager* shader = &ShaderManager::GetInstance();

	SHADER_WORLD world;
	world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
	world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
	shader->UpdateBuffer("MainWorld", &world);

	shader->SetTexturePS(g_pTexture);

	DrawPolygon();
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void SetPolygonTexture(ID3D11ShaderResourceView* pTexture)
{
	g_pTexture = pTexture;
	g_mTex._44 = (g_pTexture) ? 1.0f : 0.0f;
}

//=============================================================================
// �\�����W�̐ݒ�
//=============================================================================
void SetPolygonPos(float fX, float fY)
{
	g_posPolygon.x = fX;
	g_posPolygon.y = fY;
}

//=============================================================================
// �\���T�C�Y�̐ݒ�
//=============================================================================
void SetPolygonSize(float fScaleX, float fScaleY)
{
	g_sizPolygon.x = fScaleX;
	g_sizPolygon.y = fScaleY;
}

//=============================================================================
// �\���p�x�̐ݒ�(�P��:�x)
//=============================================================================
void SetPolygonAngle(float fAngle)
{
	g_rotPolygon.z = fAngle;
}
void SetPolygonAngle(float3 angle)
{
	g_rotPolygon = angle;
}

//=============================================================================
// ����e�N�X�`�����W�̐ݒ� (0.0��fU��1.0, 0.0��fV��1.0)
//=============================================================================
void SetPolygonUV(float fU, float fV)
{
	g_posTexFrame.x = fU;
	g_posTexFrame.y = fV;
}

//=============================================================================
// �e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
//=============================================================================
void SetPolygonFrameSize(float fWidth, float fHeight)
{
	g_sizTexFrame.x = fWidth;
	g_sizTexFrame.y = fHeight;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetPolygonColor(float fRed, float fGreen, float fBlue)
{
	if (fRed != g_colPolygon.x || fGreen != g_colPolygon.y || fBlue != g_colPolygon.z) {
		g_colPolygon.x = fRed;
		g_colPolygon.y = fGreen;
		g_colPolygon.z = fBlue;
		g_bInvalidate = true;
	}
}

//=============================================================================
// �s�����x�̐ݒ�
//=============================================================================
void SetPolygonAlpha(float fAlpha)
{
	if (fAlpha != g_colPolygon.w) {
		g_colPolygon.w = fAlpha;
		g_bInvalidate = true;
	}
}