/**
 * @file ModelManager.cpp
 */


/**
 * @include
 */
#include "ModelManager.h"
#include "FbxModel.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Light.h"
#include "debugproc.h"
#include "ShaderManager.h"
#include "GameObject.h"
#include "GameObject3D.h"
#include "TPCamera.h"
#include "input.h"
#include "ShaderBufferManager.h"
#include "System.h"


//------------- �m��̐ݒ�
// blender�ݒ�
// FBX�ŃG�N�X�|�[�g
// �o�C�i���f�[�^
// �g�k�� 0.01�{
// Y����


//-------------	���m��̐ݒ�
// �X�P�[��
//	�S�ă��[�J��
//	�S��FBX
// Z���O��



/**
 * @const
 */
static const char* name[E_MODEL_MAX] = {
	"data/model/none.fbx",				// �{�b�N�X
	"data/model/Sword And Shield Idle.fbx",	// �v���C���[
	//"data/model/Squadron/X_wing.fbx",	// �v���C���[
	"data/model/Squadron/X_wing.fbx",	// �G
	"data/model/FA-18/sparrow.fbx",		// �~�T�C��
	"data/model/laser.fbx",				// ���[�U�[
	"data/model/Land/Land.fbx",			// �n�`
	"data/model/sky.fbx",				// �X�J�C�h�[��
};


Light			m_lightOff;		// ��������


ModelManager::ModelManager() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		SAFE_DELETE(m_pModel[i]);
	}
	// ����������
	m_lightOff.m_direction = XMFLOAT3(0.f, 0.f, 0.f);
}


ModelManager::~ModelManager() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		SAFE_DELETE(m_pModel[i]);
	}
}


void ModelManager::Init() {
	HRESULT hr;
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		m_pModel[i] = new CFbxModel();
		hr = m_pModel[i]->Init(pDevice, pDeviceContext, name[i]);
		if (FAILED(hr)) {
			MessageBoxA(NULL, name[i], "Failed Load Model", MB_OK | MB_ICONWARNING | MB_TOPMOST);
		}
		int nStack = m_pModel[i]->GetMaxAnimStack();
		if (nStack > 0) {
			m_pModel[i]->SetAnimStack(nStack - 1);
		}
		m_nAnimFrame[i] = 0;
	}
}


void ModelManager::Uninit() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		SAFE_DELETE(m_pModel[i]);
	}
}


CFbxModel* ModelManager::Get(E_MODEL model) {
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return nullptr;
	}
	return m_pModel[model];
}


void ModelManager::Update(GameObject3D *obj) {
	E_MODEL model = obj->m_model;
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}
	// �J�������W�𔽉f
	if (++m_nAnimFrame[model] >= m_pModel[model]->GetMaxAnimFrame()) {
		m_nAnimFrame[model] = 0;
	}
}


void ModelManager::Draw(GameObject3D* obj) {

	E_MODEL model = obj->m_model;
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}

	// �}�e���A�����𔽉f
	m_pModel[model]->SetMaterial(&obj->m_material);

	// �g�p����ϐ�
	D3DClass* d3dClass = &D3DClass::GetInstance();
	ID3D11Device* pDevice = d3dClass->GetDevice();
	ID3D11DeviceContext* pDeviceContext = d3dClass->GetDeviceContext();
	CCamera* pCamera = CCamera::Get();

	// �V�F�[�_�̓K�p
	//ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	ShaderManager::GetInstance().Bind(obj->m_shader);
	pDeviceContext->HSSetShader(NULL, NULL, 0);
	pDeviceContext->DSSetShader(NULL, NULL, 0);
	pDeviceContext->GSSetShader(NULL, NULL, 0);
	pDeviceContext->CSSetShader(NULL, NULL, 0);

	// �V�F�[�_�̐ݒ�
	SHADER_LIGHT_SETTING buf;
	buf.light = (obj->m_isLight) ? XMFLOAT4(1.f, 1.f, 1.f, 1.f) : XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	ShaderBufferManager::GetInstance().Update("MainLightSetting", &buf);
	ShaderBufferManager::GetInstance().Bind("MainLightSetting");

	//--- FBX�t�@�C���\��

	// �t���[���X�V
	m_pModel[model]->SetAnimFrame(m_nAnimFrame[model]);
	// �`��
	d3dClass->SetBlendState(BS_NONE);		// �A���t�@�������Ȃ�
	d3dClass->SetZWrite(true);			// Z�o�b�t�@�L��
	if (obj->m_model == E_MODEL_SKY) { d3dClass->SetZWrite(false); }
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);
	if (model == E_MODEL_SKY) { return; }
	d3dClass->SetZWrite(false);
	d3dClass->SetBlendState(BS_ALPHABLEND);	// �������`��
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);
}


// EOF