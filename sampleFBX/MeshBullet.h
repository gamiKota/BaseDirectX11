//=============================================================================
//
// ビルボード弾 [bullet.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include "Component.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT InitBullet(void);
//void UninitBullet(void);
//void UpdateBullet(void);
//void DrawBullet(void);
//int FireBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);


/**
 * @class MeshBullet
 */
class MeshBullet :  public Component {
public:
	void Awake();
	void Uninit();
	void Update();
	void Draw();
	int Fire(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
};


// EOF