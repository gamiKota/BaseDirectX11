// 敵機クラス [Enemy.h]
#pragma once
#include "main.h"
#include "Component.h"

class CEnemy : public Component {
public:

	int m_nStat;				// 0:未使用, 1:使用中, 2:爆発中
	int m_nLife;				// 寿命

public:
	CEnemy();		// コンストラクタ
	~CEnemy();		// デストラクタ

	void Start();	// 初期化
	void Uninit();	// 終了処理
	void Update();	// 更新
};
