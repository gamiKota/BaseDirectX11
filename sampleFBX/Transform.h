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
	float3		m_position;		//!< ���W
	Quaternion	m_rotate;		//!< ��]
	float3		m_scale;		//!< �g�k
	float3		m_forward;		//!< �O����
	float3		m_right;		//!< �E����
	float3		m_up;			//!< �����

	//std::list<Tween*> m_tween;	//!< tween���X�g
	
private:
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


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

