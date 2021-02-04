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
	E_MODEL				m_model;
	float3				m_vCenter;		//!< ���E�{�b�N�X���S���W
	float3				m_vScale;		//!< ���E�{�b�N�X �T�C�Y
	bool				m_bHit;			//!< �Փ˗L��
	std::list<std::string> m_selfTag;	//!< �Z���t�^�O
	Material*			m_material;		//!< �}�e���A��


private:

public:
	Collision();

	void Awake();
	void Start();
	void Uninit();
	void SetImGuiVal();

	void DebugDraw();
	DirectX::XMFLOAT4X4 GetWorld();

	inline void SetHit() { m_bHit = true; }

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
};


//void InitCollisionShader();


// EOF