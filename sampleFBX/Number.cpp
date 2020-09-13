//=============================================================================
//
// ���l�\������ [number.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "Number.h"
#include "polygon.h"
#include "Texture.h"
#include "D3DClass.h"
#include "System.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUMBER_TEXTURENAME	L"data/texture/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define NUMBER_FRAME_X		10	// ���t���[����
#define NUMBER_FRAME_Y		1	// �c�t���[����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;			// �e�N�X�`���ւ̃|�C���^

//=============================================================================
// ���l�\���̏���������
//=============================================================================
void InitNumber(void)
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, NUMBER_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) {
		MessageBoxA(System::GetInstance().GetWnd(), "error\nload texture", "Number.cpp", MB_OK | MB_ICONERROR | MB_TOPMOST);
	}
}

//=============================================================================
// ���l�\���̏I������
//=============================================================================
void UninitNumber(void)
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// ���l�\��
//=============================================================================
void DrawNumber(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
	float fSizeX, float fSizeY)
{
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	SetPolygonTexture(g_pTexture);
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonFrameSize(1.0f / NUMBER_FRAME_X, 1.0f / NUMBER_FRAME_Y);
	float fX = pPos->x + (nWidth - 0.5f) * fSizeX;
	float fY = pPos->y - fSizeY * 0.5f;
	for (int i = 0; i < nWidth; ++i) {
		SetPolygonPos(fX, fY);
		unsigned n = uNumber % 10;
		uNumber /= 10;
		int u = n % NUMBER_FRAME_X;
		int v = n / NUMBER_FRAME_X;
		SetPolygonUV(u / (float)NUMBER_FRAME_X, v / (float)NUMBER_FRAME_Y);
		DrawPolygon(pDeviceContext);
		fX -= fSizeX;
	}
	// �e�N�X�`�� �T�C�Y�A�ʒu�����ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}
