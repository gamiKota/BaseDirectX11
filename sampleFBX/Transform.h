#pragma once


#include "Component.h"
#include "Data.h"


class Transform : public Component {
public:
	float3	m_position;		//!< ���W
	float3	m_rotate;		//!< ��]
	float3	m_scale;		//!< �g�k
	float3	m_forward;		//!< �O����
	float3	m_right;		//!< �E����
	float3	m_up;			//!< �����

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


public:
	Transform();
	void LastUpdate();

	void LookAt(Transform* target);

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

