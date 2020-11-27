#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "PixelShader.h"


enum E_SHADER {
	E_SHADER_FBX,
	E_SHADER_MONOCHROME,
	E_SHADER_MAX,
};



class ShaderManager : public Singleton<ShaderManager> {
public:
	void Initialize();
	void Terminate();

	void Bind(E_SHADER shader = E_SHADER_FBX);

private:
	VertexShader*	m_VS[E_SHADER_VS_MAX];
	PixelShader*	m_PS[E_SHADER_PS_MAX];
};


// EOF