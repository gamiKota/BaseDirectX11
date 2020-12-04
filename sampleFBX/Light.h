#pragma once


#include <DirectXMath.h>


//
// FBXƒ‚ƒfƒ‹ŒõŒ¹
//
class Light
{
public:
	Light();
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;
};


void InitLight();
void UninitLight();
void UpdateLight();
void DrawLight();

void SetLightEnable(bool);


Light* GetMainLight();


// EOF