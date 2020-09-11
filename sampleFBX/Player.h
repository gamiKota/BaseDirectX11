// ���@�N���X [Player.h]
#pragma once
#include "Component.h"
#include "Transform.h"


static const float MAX_MOVE_WIDTH = 1000.f;
static const float VAL_MOVE_PLAYER = 0.f;

class Collision;
class GameObject;

class CPlayer : public Component
{
public:
	float3			m_vMove;	//!< �ړ���
	float			m_roll;		//!< �X��(Z��)
	GameObject*		m_target;	//!< �^�[�Q�b�g

public:
	CPlayer();		// �R���X�g���N�^
	~CPlayer();		// �f�X�g���N�^

	void Awake();
	void Start();	// ������
	void Uninit();	// �I������
	void Update();	// �X�V
};
