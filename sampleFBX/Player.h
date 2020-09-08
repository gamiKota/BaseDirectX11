// ���@�N���X [Player.h]
#pragma once
#include "Component.h"


static const float MAX_MOVE_WIDTH = 500.f;
static const float VAL_MOVE_PLAYER = 2.f;

class Collision;

class CPlayer : public Component
{
public:
	DirectX::XMFLOAT3	m_vMove;	//!< �ړ���
	float				m_roll;		//!< �X��(Z��)

public:
	CPlayer();		// �R���X�g���N�^
	~CPlayer();		// �f�X�g���N�^

	void Awake();
	void Start();	// ������
	void Uninit();	// �I������
	void Update();	// �X�V
};
