/**
 * @file Transform.h
 */


#pragma once


#include <list>
#include "Component.h"
#include "Data.h"


class Tween;


class Transform : public Component {
public:
	float3		m_position;		//!< À•W
	Quaternion	m_rotate;		//!< ‰ñ“]
	float3		m_scale;		//!< Šgk
	float3		m_forward;		//!< ‘O•ûŒü
	float3		m_right;		//!< ‰E•ûŒü
	float3		m_up;			//!< ã•ûŒü

	Tween*		m_tween;		//!< tween
	
private:
	DirectX::XMFLOAT4X4 m_world;		//!< ƒ[ƒ‹ƒhs—ñ


public:
	Transform();
	void Update();
	void LastUpdate();
	void Uninit();

	Tween* DOMove(float3 position, float time = 0.f);

	void LookAt(Transform* target);
	void LookAtA(Transform* target);

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

