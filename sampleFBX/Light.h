/**
 * @file Light.h
 */


#pragma once


#include <DirectXMath.h>
#include <d3d11.h>
#include <list>
#include "Component.h"


class GameObject3D;


/**
 * @class Light : inheritance Component
 */
class Light : public Component {
private:
	static Light* m_pLight;	//!< メインライト

private:
	ID3D11ShaderResourceView* m_pRTTex;
	ID3D11RenderTargetView* m_pRTView;
	ID3D11DepthStencilView* m_pDSView;
	DirectX::XMFLOAT4X4 m_lightView;
	int m_step;

public:
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	float3 m_direction;

public:
	Light();
	void Uninit();
	void Awake();
	void Start();
	void Update();
	void LastUpdate();
	void SetImGuiVal();

	static void Set(Light* light) { m_pLight = light; }
	static Light* Get() { return m_pLight; }

	void Shadow(std::list<GameObject3D*> shadowObj);
};


// EOF