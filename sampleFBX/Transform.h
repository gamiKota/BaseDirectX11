/**
 * @file Transform.h
 */


#pragma once


#include <list>
#include "Component.h"
#include "Data.h"


class Tween;
class GameObject;

/**
 * @class Transform : inheritance Component
 */
class Transform : public Component {
public:
	float3		m_position;		//!< ���W
	Quaternion	m_rotate;		//!< ��]
	float3		m_scale;		//!< �g�k
	float3		m_forward;		//!< �O����
	float3		m_right;		//!< �E����
	float3		m_up;			//!< �����

	Tween*		m_tween;		//!< tween
	
	GameObject* m_Parent;	//!< �e�I�u�W�F�N�g

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


public:
	Transform();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();

	Tween* DOMove(float3 position, float time = 0.f);

	void LookAt(Transform* target, float angle = 0.f);

	DirectX::XMFLOAT4X4 GetMatrixBillboard();

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

