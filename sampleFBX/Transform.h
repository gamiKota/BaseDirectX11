#pragma once


#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component {
public:
	//DirectX::XMFLOAT3	m_position;		//!< ���W
	//DirectX::XMFLOAT3	m_rotate;		//!< ��]
	//DirectX::XMFLOAT3	m_scale;		//!< �g�k
	DirectX::XMFLOAT4X4 m_world;		//!< ���[���h�s��


public:
	Transform() {
		DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	}
};

