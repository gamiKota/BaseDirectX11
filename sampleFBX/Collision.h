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


/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	E_MODEL				m_model;
	float3				m_vCenter;		//!< 境界ボックス中心座標
	float3				m_vScale;		//!< 境界ボックス サイズ
	bool				m_bHit;			//!< 衝突有無
	std::list<std::string> m_selfTag;	//!< セルフタグ


private:

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();
	void OnCollision(GameObject* obj);

	void DebugDraw();
	DirectX::XMFLOAT4X4 GetWorld();

	inline void SetHit() { m_bHit = true; }

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
};


//void InitCollisionShader();


// EOF