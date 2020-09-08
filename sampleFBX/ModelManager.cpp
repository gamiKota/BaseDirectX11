/**
 * @file ModelManager.cpp
 */


/**
 * @include
 */
#include "ModelManager.h"
#include "FbxModel.h"
#include "System.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Light.h"
#include "System.h"


/**
 * @const
 */
static const char* name[E_MODEL_MAX] = {
	"model_none",
	"data/model/X_wing.fbx",		// �v���C���[
	"data/model/X_wing.fbx",		// �G
	"data/model/FA-18/sparrow.fbx",	// �~�T�C��
	"data/model/sky.fbx",			// �X�J�C�h�[��
};


extern CFbxLight	g_light;		// �������
CFbxLight			m_lightOff;	// ��������


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
	for (int i = E_MODEL_NONE + 1; i < E_MODEL_MAX; i++) {
		m_pModel[i] = new CFbxModel();
		hr = m_pModel[i]->Init(pDevice, pDeviceContext, name[i]);
		if (SUCCEEDED(hr)) {
			//m_pModel[i]->SetCamera(CCamera::Get()->GetEye());
			if (i == E_MODEL_SKY) {
				m_pModel[i]->SetLight(m_lightOff);
			}
			else {
				m_pModel[i]->SetLight(g_light);
			}
		}
		else {
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
	if (model <= E_MODEL_NONE || model > E_MODEL_MAX) {
		return nullptr;
	}
	return m_pModel[model];
}


void ModelManager::Update(E_MODEL model) {
	if (model <= E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}
	// �J�������W�𔽉f
	m_pModel[model]->SetCamera(CCamera::Get()->GetEye());
	// ��������𔽉f
	if (model == E_MODEL_SKY) {
		m_pModel[model]->SetLight(m_lightOff);
	}
	else {
		m_pModel[model]->SetLight(g_light);
	}
}


void ModelManager::Draw(E_MODEL model, XMFLOAT4X4 transform) {

	if (model <= E_MODEL_NONE || model > E_MODEL_MAX) {
		return ;
	}

	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	CCamera* pCamera = CCamera::Get();
	// FBX�t�@�C���\��
	D3DClass::GetInstance().SetBlendState(BS_NONE);			// �A���t�@�������Ȃ�
	m_pModel[model]->Render(transform, pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);
	D3DClass::GetInstance().SetZWrite(false);
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);	// �������`��
	m_pModel[model]->Render(transform, pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);

	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);	// �O�ʃJ�����O(�\��`���Ȃ�)
	D3DClass::GetInstance().SetZWrite(true);
}


// EOF