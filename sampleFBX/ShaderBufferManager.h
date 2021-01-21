#pragma once

#include <Windows.h>
#include <map>
#include <DirectXMath.h>
#include "Singleton.h"

#define MAX_BONE_MATRIX	256

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
// bool�^�Ȃ�Ń_���Ȃ�
struct SHADER_LIGHT_SETTING {
	//bool light;
	//bool dummy[15];
	DirectX::XMFLOAT4 light;
};

// �`��I�u�W�F�N�g�̍s����
struct SHADER_WORLD {
	DirectX::XMMATRIX mWorld;	// ���[���h�s��
	DirectX::XMMATRIX mTexture;	// �e�N�X�`���s��
};

// �}�e���A�����
struct SHADER_MATERIAL {
	DirectX::XMVECTOR	vAmbient;	// �A���r�G���g�F
	DirectX::XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	DirectX::XMVECTOR	vSpecular;	// �X�y�L�����F
	DirectX::XMVECTOR	vEmissive;	// �G�~�b�V�u�F
};

// �V�F�[�_�ɓn���{�[���s��z��
struct SHADER_BONE {
	DirectX::XMMATRIX mBone[MAX_BONE_MATRIX];
	SHADER_BONE()
	{
		for (int i = 0; i < MAX_BONE_MATRIX; i++) {
			mBone[i] = DirectX::XMMatrixIdentity();
		}
	}
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