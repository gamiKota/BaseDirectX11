#pragma once


#include "Component.h"
#include "Data.h"


typedef struct {
	float3	start;
	float3	end;
	float	time;
} Interpolation;


class Transform : public Component {
public:
	float3		m_position;		//!< 座標
	Quaternion	m_rotate;		//!< 回転
	float3		m_scale;		//!< 拡縮
	float3		m_forward;		//!< 前方向
	float3		m_right;		//!< 右方向
	float3		m_up;			//!< 上方向
	float3		m_eulerAngles;	//!< オイラー角

	Interpolation m_DORotate;	//!< DOTween出来たらいいなぁ

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ワールド行列


public:
	Transform();
	void Update();
	void LastUpdate();

	void LookAt(Transform* target);
	void LookAtA(Transform* target);

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

