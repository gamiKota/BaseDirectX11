// 自機クラス [Player.h]
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
	float3			m_vMove;	//!< 移動量
	float			m_roll;		//!< 傾き(Z軸)
	GameObject*		m_target;	//!< ターゲット

public:
	CPlayer();		// コンストラクタ
	~CPlayer();		// デストラクタ

	void Awake();
	void Start();	// 初期化
	void Uninit();	// 終了処理
	void Update();	// 更新
};
