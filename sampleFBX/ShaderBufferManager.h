#pragma once

#include <Windows.h>
#include <map>
#include <DirectXMath.h>
#include "Singleton.h"


class ShaderBuffer;


// �J�����̒萔�o�b�t�@
struct SHADER_CAMERA {
	DirectX::XMVECTOR	vEye;	// ���_���W
	DirectX::XMMATRIX	mV;		// �r���[
	DirectX::XMMATRIX	mP;		// �v���W�F�N�V����
};


// ���C�g�̒萔�o�b�t�@
struct SHADER_LIGHT {
	DirectX::XMVECTOR	vLightDir;	// ��������
	DirectX::XMVECTOR	vLa;		// �����F(�A���r�G���g)
	DirectX::XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	DirectX::XMVECTOR	vLs;		// �����F(�X�y�L����)
};

// ���C�g��ON/OFF(1/16)
struct SHADER_LIGHT_SETTING {
	DirectX::XMFLOAT4 light;
};


class ShaderBufferManager : public Singleton<ShaderBufferManager> {
public:
	std::map<std::string, int> m_registerMap;
	std::map<std::string, ShaderBuffer *> m_bufferMap;

	void Initialize();
	void Terminate();

	void Update(std::string bufName, void* pData);
	void Bind(std::string bufName);

private:
	void Create(std::string bufName, UINT size, int regNum);
};