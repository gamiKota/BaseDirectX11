/**
 * @file Light.h
 */


#pragma once


#include <DirectXMath.h>
#include "Component.h"


/**
 * @class Light : inheritance Component
 */
class Light : public Component
{
private:
	static Light* m_pLight;	//!< ƒƒCƒ“ƒ‰ƒCƒg

public:
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;

public:
	Light();

	void Awake();
	void Start();
	void Update();
	void LastUpdate();

	static void Set(Light* light) { m_pLight = light; }
	static Light* Get() { return m_pLight; }
};


// EOF