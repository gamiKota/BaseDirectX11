#pragma once


#include <DirectXMath.h>

class Transform {
public:
	DirectX::XMFLOAT4X4 m_world;

public:
	Transform() {
		DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	}
};

