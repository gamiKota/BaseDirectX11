#pragma once


#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component {
public:
	DirectX::XMFLOAT3	m_position;		//!< ���W
	DirectX::XMFLOAT3	m_rotate;		//!< ��]
	DirectX::XMFLOAT3	m_scale;		//!< �g�k
	DirectX::XMFLOAT3	m_forward;		//!< �O����

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


public:
	Transform();
	void LastUpdate();

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

