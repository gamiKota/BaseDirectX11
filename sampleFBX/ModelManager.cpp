/**
 * @file ModelManager.cpp
 */


/**
 * @include
 */
#include "ModelManager.h"
#include "FbxModel.h"
#include "D3DClass.h"
#include "debugproc.h"
#include "GameObject3D.h"
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
	//"data/model/Sword And Shield Idle.fbx",	// プレイヤー
	"data/model/Squadron/X_wing.fbx",	// プレイヤー
	"data/model/Squadron/X_wing.fbx",	// 敵
	"data/model/FA-18/sparrow.fbx",		// ミサイル
	"data/model/laser.fbx",				// レーザー
	"data/model/Land/Land.fbx",			// 地形
	"data/model/sky.fbx",				// スカイドーム
};


ModelManager::ModelManager() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		SAFE_DELETE(m_pModel[i]);
	}
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

	//--- FBXファイル表示
	// フレーム更新
	m_pModel[model]->SetAnimFrame(m_nAnimFrame[model]);
	// 描画
	d3dClass->SetBlendState(BS_NONE);		// アルファ処理しない
	d3dClass->SetZWrite(true);			// Zバッファ有効
	if (obj->m_model == E_MODEL_SKY) { d3dClass->SetZWrite(false); }
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), eOpacityOnly);
	if (model == E_MODEL_SKY) { return; }
	d3dClass->SetZWrite(false);
	d3dClass->SetBlendState(BS_ALPHABLEND);	// 半透明描画
	m_pModel[model]->Render(obj->m_transform->GetMatrix(), eTransparentOnly);
}


// EOF