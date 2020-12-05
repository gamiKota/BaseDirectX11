/**
 * @file MeshBullet.h
 */
#pragma once


/**
 * @include
 */
#include <DirectXMath.h>
#include "Component.h"


/**
 * @class MeshBullet : inheritance Component
 */
class MeshBullet :  public Component {
private:

public:
	void Awake();
	void Update();
	void Fire(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
};


// EOF