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
	DirectX::XMFLOAT3	m_vCenter;		//!< 境界ボックス中心座標
	DirectX::XMFLOAT3	m_vBBox;		//!< 境界ボックス サイズ
	DirectX::XMFLOAT3	m_vPosBBox;		//!< 境界ボックス中心座標(ワールド空間)
	DirectX::XMFLOAT4X4	m_world;		//!< 当たり判定用行列
	bool				m_bHit;			//!< 衝突有無

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