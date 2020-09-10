#pragma once


#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component {
public:
	DirectX::XMFLOAT3	m_position;		//!< À•W
	DirectX::XMFLOAT3	m_rotate;		//!< ‰ñ“]
	DirectX::XMFLOAT3	m_scale;		//!< Šgk
	DirectX::XMFLOAT3	m_forward;		//!< ‘O•ûŒü

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ƒ[ƒ‹ƒhs—ñ


public:
	Transform();
	void LastUpdate();

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

