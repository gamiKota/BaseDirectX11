#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>


enum E_SHADER {
	E_SHADER_FBX,
	E_SHADER_DEFAULT,
	E_SHADER_TOON,
	E_SHADER_OUTLINE,
	E_SHADER_MONOCHROME,
	E_SHADER_PHONG,
	E_SHADER_MAX,
};



class ShaderManager : public Singleton<ShaderManager> {
public:
	void Initialize();
	void Terminate();

	void Bind(E_SHADER shader = E_SHADER_FBX, E_SHADER_GS GS = E_SHADER_GS_DEFAULT);

	void UpdateBuffer(DirectX::XMFLOAT4X4 world);

private:
	VertexShader*	m_VS[E_SHADER_VS_MAX];
	GeometryShader* m_GS[E_SHADER_GS_MAX];
	PixelShader*	m_PS[E_SHADER_PS_MAX];
};


// EOF