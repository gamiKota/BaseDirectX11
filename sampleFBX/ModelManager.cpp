/**
 * @file ModelManager.cpp
 */


/**
 * @include
 */
#include "ModelManager.h"
#include "D3DClass.h"
#include "debugproc.h"
#include "GameObject3D.h"
#include "FBX/FBXPlayer.h"
#include "DrawBuffer.h"
#include "FBX/FBXLoader.h"
#include "Geometory.h"
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

}


ModelManager::~ModelManager() {

}


void ModelManager::Init() {

	ggfbx::Initialize();
	InitGeometory();

	// モデル
	for (int model = 0; model < E_MODEL_MAX; ++model) {
		m_pModelData[model] = new FBXPlayer;
		m_pModelData[model]->LoadModel(name[model]);
		m_pModelBuf[model] = new DrawBuffer[m_pModelData[model]->GetMeshNum()];
		for (int i = 0; i < m_pModelData[model]->GetMeshNum(); ++i)
		{
			m_pModelBuf[model][i].CreateVertexBuffer(
				m_pModelData[model]->GetVertexData(i),
				m_pModelData[model]->GetVertexSize(i),
				m_pModelData[model]->GetVertexCount(i));
			m_pModelBuf[model][i].CreateIndexBuffer(
				m_pModelData[model]->GetIndexData(i),
				sizeof(unsigned long),
				m_pModelData[model]->GetIndexCount(i));
		}
	}
}


void ModelManager::Uninit() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		delete[] m_pModelBuf[i];
		delete m_pModelData[i];
	}
	ggfbx::Terminate();
}


void ModelManager::Update(E_MODEL model) {

}


void ModelManager::Draw(E_MODEL model) {

	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}

	// 使用する変数
	D3DClass* d3dClass = &D3DClass::GetInstance();

	d3dClass->SetBlendState(EBlendState::BS_NONE);	// ブレンド
	d3dClass->SetCullMode(CULLMODE_CCW);			// カリング
	if (model != E_MODEL_SKY) { d3dClass->SetZWrite(true); }
	else { d3dClass->SetZWrite(false); }

	for (int i = 0; i < m_pModelData[model]->GetMeshNum(); ++i) {
		ShaderManager::GetInstance().SetTexturePS(m_pModelData[model][i].GetTexture(i));
		m_pModelBuf[model][i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}


// EOF