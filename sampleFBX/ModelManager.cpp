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
	"data/model/X_wing.fbx",		// プレイヤー
	"data/model/X_wing.fbx",		// 敵
	"data/model/FA-18/sparrow.fbx",	// ミサイル
	"data/model/sky.fbx",			// スカイドーム
};


extern CFbxLight	g_light;		// 光源情報
CFbxLight			m_lightOff;	// 光源無効


ModelManager::ModelManager() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		SAFE_DELETE(m_pModel[i]);
	}
	// 光源無効化
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
	// カメラ座標を反映
	m_pModel[model]->SetCamera(CCamera::Get()->GetEye());
	// 光源上方を反映
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
	// FBXファイル表示
	D3DClass::GetInstance().SetBlendState(BS_NONE);			// アルファ処理しない
	m_pModel[model]->Render(transform, pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);
	D3DClass::GetInstance().SetZWrite(false);
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);	// 半透明描画
	m_pModel[model]->Render(transform, pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);

	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);	// 前面カリング(表を描かない)
	D3DClass::GetInstance().SetZWrite(true);
}


// EOF