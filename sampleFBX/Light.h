/**
 * @file Light.h
 */


#pragma once


#include <DirectXMath.h>
#include "Component.h"


 // ���C�g�̃V�F�[�_�o�b�t�@
struct SHADER_LIGHT {
	DirectX::XMVECTOR	vLightDir;	// ��������
	DirectX::XMVECTOR	vLa;		// �����F(�A���r�G���g)
	DirectX::XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	DirectX::XMVECTOR	vLs;		// �����F(�X�y�L����)
};


/**
 * @class Light : inheritance Component
 */
class Light : public Component
{
private:
	static Light* m_pLight;	//!< ���C�����C�g
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