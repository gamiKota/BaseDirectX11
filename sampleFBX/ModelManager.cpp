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
	"data/model/box.fbx",				// ボックス
	"data/model/Squadron/X_wing.fbx",	// プレイヤー
	"data/model/Squadron/X_wing.fbx",	// 敵
	"data/model/FA-18/sparrow.fbx",		// ミサイル
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

	TFbxMaterial material;
	//material = *m_pModel[model]->GetMaterial();
	material.Ka = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値はテクスチャrgbはモデル自体の色
	material.Ke = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値を０にすると真っ白 
	material.Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 値を小さくするとモデルが薄くなる
	material.Ks = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// 光沢

	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		m_pModel[i] = new CFbxModel();
		hr = m_pModel[i]->Init(pDevice, pDeviceContext, name[i]);
		if (SUCCEEDED(hr)) {
			//m_pModel[i]->SetCamera(GameObject::Find("MainCamera")->GetComponent<TPCamera>()->GetEye());
			//m_pModel[i]->SetCamera(CCamera::Get()->GetEye());
			if (i == E_MODEL_SKY) {
				m_pModel[i]->SetLight(m_lightOff);
			}
			else {
				m_pModel[i]->SetLight(*Light::Get());
			}
			m_pModel[i]->SetMaterial(&material);
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
	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return nullptr;
	}
	return m_pModel[model];
}


void ModelManager::Update(E_MODEL model) {
	if (model <= E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}
	// カメラ座標を反映
	m_pModel[model]->SetCamera(CCamera::Get()->m_transform->m_position);
	// 光源上方を反映
	if (model == E_MODEL_SKY) {
		m_pModel[model]->SetLight(m_lightOff);
	}
	else {
		m_pModel[model]->SetLight(*Light::Get());
	}
}


void ModelManager::Draw(GameObject3D* obj) {

	E_MODEL model = obj->m_model;
	if (model <= E_MODEL_NONE || model > E_MODEL_MAX) {
		return ;
	}

	// 使用する変数
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	CCamera* pCamera = CCamera::Get();

	// シェーダの適用
	ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	ShaderManager::GetInstance().Bind(obj->m_shader);

	//--- FBXファイル表示

	D3DClass::GetInstance().SetBlendState(BS_NONE);		// アルファ処理しない
	D3DClass::GetInstance().SetZWrite(true);			// Zバッファ有効

	if (obj->m_model == E_MODEL_SKY) {
		D3DClass::GetInstance().SetZWrite(false);
	}
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eOpacityOnly);

	if (model == E_MODEL_SKY) {
		D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);		// アルファ処理しない
		D3DClass::GetInstance().SetZWrite(true);					// Zバッファ有効
		return;
	}

	D3DClass::GetInstance().SetZWrite(false);
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);	// 半透明描画
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eTransparentOnly);

	// エッジ検出のやり方じゃないと複雑なモデル描画時に
	// カリングの設定だけでは足りずに黒い部分が描画されてしまう
	if (obj->m_shader == E_SHADER_TOON) {
		// シェーダの適用
		D3DClass::GetInstance().SetBlendState(BS_NONE);		// アルファ処理しない
		D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);	// 前面カリング (FBXは表裏が反転するため)
		D3DClass::GetInstance().SetZWrite(true);			// Zバッファ有効
		ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
		ShaderManager::GetInstance().Bind(E_SHADER_OUTLINE);
		m_pModel[model]->Render(obj->m_transform->GetMatrix(), pCamera->GetView(), pCamera->GetProj(), eNoAffect);
	}
}


// EOF