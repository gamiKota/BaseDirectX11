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
#include "System.h"


/**
 * @const
 */
static const char* name[E_MODEL_MAX] = {
	"data/model/box.fbx",				// �{�b�N�X
	"data/model/Squadron/X_wing.fbx",	// �v���C���[
	"data/model/Squadron/X_wing.fbx",	// �G
	"data/model/FA-18/sparrow.fbx",		// �~�T�C��
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
			MessageBoxA(System::GetInstance().GetWnd(), name[i], "Failed Load Model", MB_OK | MB_ICONWARNING | MB_TOPMOST);
		}
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
	m_pModel[model]->SetCamera(CCamera::Get()->m_transform->m_position);
	// ��������𔽉f
	if (obj->m_isLight) {
		m_pModel[model]->SetLight(*Light::Get());
	}
	else {
		m_pModel[model]->SetLight(m_lightOff);
	}
}


void ModelManager::Draw(GameObject3D* obj) {

	E_MODEL model = obj->m_model;
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return ;
	}

	m_pModel[model]->SetMaterial(&obj->m_material);

	// �g�p����ϐ�
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	CCamera* pCamera = CCamera::Get();

	// �V�F�[�_�̓K�p
	ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	ShaderManager::GetInstance().Bind(obj->m_shader);

	//--- FBX�t�@�C���\��

	D3DClass::GetInstance().SetBlendState(BS_NONE);		// �A���t�@�������Ȃ�
	D3DClass::GetInstance().SetZWrite(true);			// Z�o�b�t�@�L��

	if (obj->m_model == E_MODEL_SKY) {
		D3DClass::GetInstance().SetZWrite(false);
	}
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);

	if (model == E_MODEL_SKY) {
		D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);		// �A���t�@�������Ȃ�
		D3DClass::GetInstance().SetZWrite(true);					// Z�o�b�t�@�L��
		return;
	}

	D3DClass::GetInstance().SetZWrite(false);
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);	// �������`��
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);

	// �G�b�W���o�̂�������Ȃ��ƕ��G�ȃ��f���`�掞��
	// �J�����O�̐ݒ肾���ł͑��肸�ɍ����������`�悳��Ă��܂�
	//if (obj->m_shader == E_SHADER_TOON) {
	//	// �V�F�[�_�̓K�p
	//	D3DClass::GetInstance().SetBlendState(BS_NONE);		// �A���t�@�������Ȃ�
	//	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	//	D3DClass::GetInstance().SetZWrite(true);			// Z�o�b�t�@�L��
	//	ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	//	ShaderManager::GetInstance().Bind(E_SHADER_OUTLINE);
	//	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eNoAffect);
	//}
}


// EOF