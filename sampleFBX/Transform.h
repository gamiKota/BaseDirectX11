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
	float3		m_position;		//!< ���W
	Quaternion	m_rotate;		//!< ��]
	float3		m_scale;		//!< �g�k
	float3		m_forward;		//!< �O����
	float3		m_right;		//!< �E����
	float3		m_up;			//!< �����

	Tween*		m_tween;		//!< tween
	
private:
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


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

