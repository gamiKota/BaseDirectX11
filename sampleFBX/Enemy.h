// �G�@�N���X [Enemy.h]
#pragma once
#include "main.h"
#include "Component.h"

class CEnemy : public Component {
public:

	int m_nStat;				// 0:���g�p, 1:�g�p��, 2:������
	int m_nLife;				// ����

public:
	CEnemy();		// �R���X�g���N�^
	~CEnemy();		// �f�X�g���N�^

	void Start();	// ������
	void Uninit();	// �I������
	void Update();	// �X�V
};
