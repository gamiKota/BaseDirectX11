//=============================================================================
//
// スコア処理 [score.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "Component.h"

class Score : public Component {
public:
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャへのポインタ
	DirectX::XMFLOAT2			m_posScore;			// 位置
	int							m_nScore;			// スコア

public:
	void Awake();
	void Start();
	void Uninit();
	void Update();
	void Draw();

	int GetScore();
	void AddScore(int value);
};