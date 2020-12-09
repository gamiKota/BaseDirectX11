

#include "Light.h"
#include "D3DClass.h"
#include "System.h"


#define LIGHT0_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_AMBIENT	XMFLOAT4(0.2f,0.2f,0.2f,1.0f)
#define LIGHT0_SPECULAR	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_DIR_X	(-1.0f)
#define LIGHT0_DIR_Y	(-2.0f)
#define LIGHT0_DIR_Z	(-1.0f)


Light	g_light;
bool	g_bLightEnable = true;


Light::Light() :	m_diffuse(XMFLOAT4(1.f, 1.f, 1.f, 1.f)),
					m_ambient(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_specular(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_direction(XMFLOAT3(0.f, 0.f, 1.f))
{
}


void InitLight(void)
{
	if (g_bLightEnable) {
		XMStoreFloat3(&g_light.m_direction,
			XMVector3Normalize(XMVectorSet(LIGHT0_DIR_X, LIGHT0_DIR_Y, LIGHT0_DIR_Z, 0.0f)));
	}
	else {
		g_light.m_direction = XMFLOAT3(0, 0, 0);
	}
	g_light.m_diffuse = LIGHT0_DIFFUSE;
	g_light.m_ambient = LIGHT0_AMBIENT;
	g_light.m_specular = LIGHT0_SPECULAR;
}


void UninitLight(void) {

}


void UpdateLight(void) {

}


void DrawLight(void) {

}


void SetLightEnable(bool b) {
	g_bLightEnable = b;
}


Light* GetMainLight() {
	return &g_light;
}


// EOF