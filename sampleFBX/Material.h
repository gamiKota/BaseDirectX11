#pragma once
#include "Component.h"


class Material : public Component {
public:
	DirectX::XMFLOAT4	m_diffuse;		//!< Diffuse color RGBA
	DirectX::XMFLOAT4	m_ambient;		//!< Ambient color RGB
	DirectX::XMFLOAT4	m_specular;		//!< Specular 'shininess'
	DirectX::XMFLOAT4	m_emissive;		//!< Emissive color RGB

public:
	Material();
	void SetImGuiVal();

public:
};

