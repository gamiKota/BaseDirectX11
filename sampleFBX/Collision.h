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
	float3					m_vCenter;		//!< ���E�{�b�N�X���S���W
	float3					m_vScale;		//!< ���E�{�b�N�X �T�C�Y
	std::list<std::string>	m_selfTag;		//!< �Z���t�^�O
	Material*				m_material;		//!< �}�e���A��

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