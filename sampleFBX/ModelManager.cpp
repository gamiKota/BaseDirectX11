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


//------------- 確定の設定
// blender設定
// FBXでエクスポート
// バイナリデータ
// 拡縮率 0.01倍
// Yが上


//-------------	未確定の設定
// スケール
//	全てローカル
//	全てFBX
// Zが前方



/**
 * @const
 */
static const char* name[E_MODEL_MAX] = {
	"data/model/none.fbx",				// ボックス
	"data/model/Sword And Shield Idle.fbx",	// プレイヤー
	//"data/model/Squadron/X_wing.fbx",	// プレイヤー
	"data/model/Squadron/X_wing.fbx",	// 敵
	"data/model/FA-18/sparrow.fbx",		// ミサイル
	"data/model/laser.fbx",				// レーザー
	"data/model/Land/Land.fbx",			// 地形
	"data/model/sky.fbx",				// スカイドーム
};


Light			m_lightOff;		// 光源無効


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
	// カメラ座標を反映
	if (++m_nAnimFrame[model] >= m_pModel[model]->GetMaxAnimFrame()) {
		m_nAnimFrame[model] = 0;
	}
}


void ModelManager::Draw(GameObject3D* obj) {

	E_MODEL model = obj->m_model;
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}

	// マテリアル情報を反映
	m_pModel[model]->SetMaterial(&obj->m_material);

	// 使用する変数
	D3DClass* d3dClass = &D3DClass::GetInstance();
	ID3D11Device* pDevice = d3dClass->GetDevice();
	ID3D11DeviceContext* pDeviceContext = d3dClass->GetDeviceContext();
	CCamera* pCamera = CCamera::Get();

	// シェーダの適用
	//ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	ShaderManager::GetInstance().Bind(obj->m_shader);
	pDeviceContext->HSSetShader(NULL, NULL, 0);
	pDeviceContext->DSSetShader(NULL, NULL, 0);
	pDeviceContext->GSSetShader(NULL, NULL, 0);
	pDeviceContext->CSSetShader(NULL, NULL, 0);

	// シェーダの設定
	SHADER_LIGHT_SETTING buf;
	buf.light = (obj->m_isLight) ? XMFLOAT4(1.f, 1.f, 1.f, 1.f) : XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	ShaderBufferManager::GetInstance().Update("MainLightSetting", &buf);
	ShaderBufferManager::GetInstance().Bind("MainLightSetting");

	//--- FBXファイル表示

	// フレーム更新
	m_pModel[model]->SetAnimFrame(m_nAnimFrame[model]);
	// 描画
	d3dClass->SetBlendState(BS_NONE);		// アルファ処理しない
	d3dClass->SetZWrite(true);			// Zバッファ有効
	if (obj->m_model == E_MODEL_SKY) { d3dClass->SetZWrite(false); }
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);
	if (model == E_MODEL_SKY) { return; }
	d3dClass->SetZWrite(false);
	d3dClass->SetBlendState(BS_ALPHABLEND);	// 半透明描画
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);
}


// EOF