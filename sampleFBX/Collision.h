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


/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	DirectX::XMFLOAT4	m_color;
	DirectX::XMFLOAT3	m_vCenter;		//!< ���E�{�b�N�X���S���W
	DirectX::XMFLOAT3	m_vBBox;		//!< ���E�{�b�N�X �T�C�Y
	DirectX::XMFLOAT3	m_vPosBBox;		//!< ���E�{�b�N�X���S���W(���[���h���)
	DirectX::XMFLOAT4X4	m_world;		//!< �����蔻��p�s��
	bool				m_bHit;			//!< �Փ˗L��

private:
	bool m_isInit;

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();
	void DebugDraw();

	inline void SetHit() {
		m_bHit = true;
	}

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
	void Init(E_MODEL model);
	void SetWorld(DirectX::XMFLOAT4X4 world);
	void OnCollision(GameObject* obj);
};


//void InitCollisionShader();


// EOF