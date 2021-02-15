/**
 * @file Collision.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "ModelManager.h"
#include <d3d11.h>
#include <list>

class Material;

/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	float3					m_vCenter;		//!< 境界ボックス中心座標
	float3					m_vScale;		//!< 境界ボックス サイズ
	std::list<std::string>	m_selfTag;		//!< セルフタグ
	Material*				m_material;		//!< マテリアル

	Collision();
	~Collision();
	void Uninit();
	void SetImGuiVal();
};


/**
 * @class CollisionBox : inheritance Collision
 */
class CollisionBox : public Collision {
public:
	void DebugDraw();
	DirectX::XMFLOAT4X4 GetWorld();
	static bool AABB(CollisionBox obj1, CollisionBox obj2);
	static bool OBB(CollisionBox obj1, CollisionBox obj2);
};


// EOF