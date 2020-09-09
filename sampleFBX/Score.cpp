//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Score.h"
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "Graphics.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "System.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FRAME_SCORE	L"data/TEXTURE/frame_score.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define FRAME_SIZE_X		(340)							// �g�̃T�C�Y
#define FRAME_SIZE_Y		(80)							// �g�̃T�C�Y
#define FRAME_POS_X			(SCREEN_WIDTH/2-FRAME_SIZE_X/2)	// �g�̕\���ʒu
#define FRAME_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)	// �g�̕\���ʒu

#define SCORE_WIDTH			(8)								// �\������

#define SCORE_SIZE_X		(35)							// �����̃T�C�Y
#define SCORE_SIZE_Y		(50)							// �����̃T�C�Y
#define SCORE_POS_X			(FRAME_POS_X-SCORE_SIZE_X*(SCORE_WIDTH/2))	// �����̕\���ʒu
#define SCORE_POS_Y			(FRAME_POS_Y+SCORE_SIZE_Y/2-8)	// �����̕\���ʒu


void Score::Awake() {

}

void Score::Start() {
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	m_posScore = XMFLOAT2(SCORE_POS_X, SCORE_POS_Y);

	// �X�R�A�̏�����
	m_nScore = 0;

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FRAME_SCORE,	// �t�@�C���̖��O
		&m_pTexture);			// �ǂݍ��ރ������[
}

void Score::Uninit() {
	// �e�N�X�`���̊J��
	SAFE_RELEASE(m_pTexture);
}

void Score::Update() {

}

void Score::Draw() {
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	SetPolygonColor(0.0f, 1.0f, 0.0f);
	SetPolygonAlpha(1.0f);

	SetPolygonTexture(m_pTexture);
	SetPolygonSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
	DrawPolygon(pDeviceContext);

	DrawNumber(&m_posScore, (unsigned)m_nScore, SCORE_WIDTH);

	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

int Score::GetScore() {
	return m_nScore;
}

void Score::AddScore(int value) {
	m_nScore += value;
	if (m_nScore < 0) {
		m_nScore = 0;
	}
	else if (m_nScore >= (int)(powf(10.0f, (float)(SCORE_WIDTH + 1)))) {
		m_nScore = (int)(powf(10.0f, (float)(SCORE_WIDTH + 1))) - 1;
	}
}