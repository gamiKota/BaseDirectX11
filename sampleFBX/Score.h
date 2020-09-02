//=============================================================================
//
// �X�R�A���� [score.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "Component.h"

class Score : public Component {
public:
	ID3D11ShaderResourceView*	m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	DirectX::XMFLOAT2			m_posScore;			// �ʒu
	int							m_nScore;			// �X�R�A

public:
	void Awake();
	void Start();
	void Uninit();
	void Update();
	void Draw();

	int GetScore();
	void AddScore(int value);
};