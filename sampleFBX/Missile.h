// ホーミングミサイルクラス [Missile.h]
#pragma once
#include "Component.h"


class Bullet : public Component
{
private:
	int			m_nStat;		// 0:未使用, 1:使用中, 2:爆発中
	int			m_nLife;		// 寿命

public:
	Bullet();		// コンストラクタ
	~Bullet();	// デストラクタ

	void Awake();
	void Start();		// 初期化
	void Uninit();		// 終了処理
	void Update();		// 更新

	void OnCollision(GameObject* obj);

	void Fire(Transform transform);	// 発射


	//XMFLOAT3 GetPos();		// 座標取得
	//void SetPos(XMFLOAT3& vPos);
	//void SetWorld(XMFLOAT4X4& mWorld);
	//XMFLOAT4X4& GetWorld();	// ワールド変換取得
	//void GetBBoxInfo(XMFLOAT3& vPos, XMFLOAT3& vBox);
	//void SetHit(bool bHit);
};
