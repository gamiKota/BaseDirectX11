/**
 * @file Light.cpp
 */


/**
 * @include
 */
#include "Light.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "System.h"


/**
 * @constant
 */
#define LIGHT0_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_AMBIENT	XMFLOAT4(0.5f,0.5f,0.5f,1.0f)
#define LIGHT0_SPECULAR	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_DIR_X	(0.0f)
#define LIGHT0_DIR_Y	(-1.0f)
#define LIGHT0_DIR_Z	(0.0f)


Light* Light::m_pLight = nullptr;


Light::Light() :	m_diffuse(XMFLOAT4(1.f, 1.f, 1.f, 1.f)),
					m_ambient(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_specular(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_direction(XMFLOAT3(0.f, 0.f, 1.f))
{
}


void Light::Awake() {
	m_direction = XMFLOAT3(LIGHT0_DIR_X, LIGHT0_DIR_Y, LIGHT0_DIR_Z);
	m_diffuse	= LIGHT0_DIFFUSE;
	m_ambient	= LIGHT0_AMBIENT;
	m_specular	= LIGHT0_SPECULAR;
}


void Light::Start() {

}


void Light::Update() {

}


void Light::LastUpdate() {
	if (this == m_pLight) {
		SHADER_LIGHT buf;
		buf.vLightDir = XMLoadFloat3(&m_pLight->m_direction);
		buf.vLd = XMLoadFloat4(&m_pLight->m_diffuse);
		buf.vLa = XMLoadFloat4(&m_pLight->m_ambient);
		buf.vLs = XMLoadFloat4(&m_pLight->m_specular);
		ShaderManager::GetInstance().UpdateBuffer("MainLight", &buf);
	}
}


// EOF