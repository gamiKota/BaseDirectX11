// 自機クラス [Player.h]
#pragma once
#include "Component.h"


static const float MAX_MOVE_WIDTH = 500.f;
static const float VAL_MOVE_PLAYER = 2.f;

class Collision;

class CPlayer : public Component
{
public:
	DirectX::XMFLOAT3	m_vMove;	//!< 移動量
	float				m_roll;		//!< 傾き(Z軸)

public:
	CPlayer();		// コンストラクタ
	~CPlayer();		// デストラクタ

	void Awake();
	void Start();	// 初期化
	void Uninit();	// 終了処理
	void Update();	// 更新
};
