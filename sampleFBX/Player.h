// 自機クラス [Player.h]
#pragma once
#include "Component.h"


static const float MAX_MOVE_WIDTH = 500.f;
static const float VAL_MOVE_PLAYER = 10.f;

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
	void Draw();	// 描画

	//static XMFLOAT3 GetPos();		// 座標取得
	//static XMFLOAT4X4& GetWorld();	// ワールド変換取得
	//static void GetBBoxInfo(XMFLOAT3& vPos,
	//						XMFLOAT3& vBox);
	//static void SetHit(bool bHit);
};
