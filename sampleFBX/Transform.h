/**
 * @file Transform.h
 */


#pragma once


#include "Component.h"
#include "Data.h"
#include <list>


class Tween;


class Transform : public Component {
public:
	float3		m_position;		//!< 座標
	Quaternion	m_rotate;		//!< 回転
	float3		m_scale;		//!< 拡縮
	float3		m_forward;		//!< 前方向
	float3		m_right;		//!< 右方向
	float3		m_up;			//!< 上方向

	//std::list<Tween*> m_tween;	//!< tweenリスト
	
private:
	DirectX::XMFLOAT4X4 m_world;		//!< ワールド行列


public:
	Transform();
	void Update();
	void LastUpdate();

	//Tween* DOMove(float3 position, float time = 0.f);

	void LookAt(Transform* target);
	void LookAtA(Transform* target);

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

