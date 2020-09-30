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
	float3		m_position;		//!< À•W
	Quaternion	m_rotate;		//!< ‰ñ“]
	float3		m_scale;		//!< Šgk
	float3		m_forward;		//!< ‘O•ûŒü
	float3		m_right;		//!< ‰E•ûŒü
	float3		m_up;			//!< ã•ûŒü
	
private:
	DirectX::XMFLOAT4X4 m_world;		//!< ƒ[ƒ‹ƒhs—ñ


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

