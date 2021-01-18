/**
 * @file Light.h
 */


#pragma once


#include <DirectXMath.h>
#include "Component.h"


 // ライトのシェーダバッファ
struct SHADER_LIGHT {
	DirectX::XMVECTOR	vLightDir;	// 光源方向
	DirectX::XMVECTOR	vLa;		// 光源色(アンビエント)
	DirectX::XMVECTOR	vLd;		// 光源色(ディフューズ)
	DirectX::XMVECTOR	vLs;		// 光源色(スペキュラ)
};


/**
 * @class Light : inheritance Component
 */
class Light : public Component
{
private:
	static Light* m_pLight;	//!< メインライト
	bool m_isLight;

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

	void SetEnable(bool enable) { m_isLight = enable; }
	static void Set(Light* light) { m_pLight = light; }
	static Light* Get() { return m_pLight; }
};


// EOF