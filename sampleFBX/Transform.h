#pragma once


#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component {
public:
	DirectX::XMFLOAT3	m_position;		//!< 座標
	DirectX::XMFLOAT3	m_rotate;		//!< 回転
	DirectX::XMFLOAT3	m_scale;		//!< 拡縮
	DirectX::XMFLOAT3	m_forward;		//!< 前方向

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ワールド行列


public:
	Transform();
	void LastUpdate();

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

