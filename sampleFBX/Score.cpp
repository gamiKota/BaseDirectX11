//=============================================================================
//
// スコア処理 [score.cpp]
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
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FRAME_SCORE	L"data/TEXTURE/frame_score.png"	// 読み込むテクスチャファイル名

#define FRAME_SIZE_X		(340)							// 枠のサイズ
#define FRAME_SIZE_Y		(80)							// 枠のサイズ
#define FRAME_POS_X			(SCREEN_WIDTH/2-FRAME_SIZE_X/2)	// 枠の表示位置
#define FRAME_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)	// 枠の表示位置

#define SCORE_WIDTH			(8)								// 表示桁数

#define SCORE_SIZE_X		(35)							// 文字のサイズ
#define SCORE_SIZE_Y		(50)							// 文字のサイズ
#define SCORE_POS_X			(FRAME_POS_X-SCORE_SIZE_X*(SCORE_WIDTH/2))	// 文字の表示位置
#define SCORE_POS_Y			(FRAME_POS_Y+SCORE_SIZE_Y/2-8)	// 文字の表示位置


void Score::Awake() {

}

void Score::Start() {
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	m_posScore = XMFLOAT2(SCORE_POS_X, SCORE_POS_Y);

	// スコアの初期化
	m_nScore = 0;

	// テクスチャの読み込み
	CreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_FRAME_SCORE,	// ファイルの名前
		&m_pTexture);			// 読み込むメモリー
}

void Score::Uninit() {
	// テクスチャの開放
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