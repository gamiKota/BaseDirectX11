#pragma once


#include "Component.h"
#include "Data.h"


class Transform : public Component {
public:
	float3	m_position;		//!< 座標
	float3	m_rotate;		//!< 回転
	float3	m_scale;		//!< 拡縮
	float3	m_forward;		//!< 前方向
	float3	m_right;		//!< 右方向
	float3	m_up;			//!< 上方向

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ワールド行列


public:
	Transform();
	void LastUpdate();

	void LookAt(Transform* target);

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

