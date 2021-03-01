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
	//"data/model/Sword And Shield Idle.fbx",	// �v���C���[
	"data/model/Squadron/X_wing.fbx",	// �v���C���[
	//"data/model/none.fbx",				// �G
	"data/model/Squadron/X_wing.fbx",	// �G
	"data/model/FA-18/sparrow.fbx",		// �~�T�C��
	"data/model/laser.fbx",				// ���[�U�[
	"data/model/Land/Land.fbx",			// �n�`
	"data/model/sky.fbx",				// �X�J�C�h�[��
};


ModelManager::ModelManager() {

}


ModelManager::~ModelManager() {

}


void ModelManager::Init() {

	ggfbx::Initialize();
	InitGeometory();

	// ���f��
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

	m_pShadow = new DrawBuffer[m_pModelData[E_MODEL_PLAYER]->GetMeshNum()];
	for (int i = 0; i < m_pModelData[E_MODEL_PLAYER]->GetMeshNum(); ++i) {
		m_pShadow[i].CreateVertexBuffer(
			m_pModelData[E_MODEL_PLAYER]->GetVertexData(i),
			m_pModelData[E_MODEL_PLAYER]->GetVertexSize(i),
			m_pModelData[E_MODEL_PLAYER]->GetVertexCount(i));
		m_pShadow[i].CreateIndexBuffer(
			m_pModelData[E_MODEL_PLAYER]->GetIndexData(i),
			sizeof(unsigned long),
			m_pModelData[E_MODEL_PLAYER]->GetIndexCount(i));
		m_pShadow[i].CreateInputBuffer(sizeof(DirectX::XMMATRIX), 5);
	}
}


void ModelManager::Uninit() {
	for (int i = E_MODEL_NONE; i < E_MODEL_MAX; i++) {
		delete[] m_pModelBuf[i];
		delete m_pModelData[i];
	}
	delete[] m_pShadow;

	ggfbx::Terminate();
}


void ModelManager::Update(E_MODEL model) {

}


void ModelManager::Draw(E_MODEL model) {

	if (model < E_MODEL_NONE || model > E_MODEL_MAX) {
		return;
	}

	// �g�p����ϐ�
	D3DClass* d3dClass = &D3DClass::GetInstance();

	d3dClass->SetBlendState(EBlendState::BS_NONE);	// �u�����h
	d3dClass->SetCullMode(CULLMODE_CCW);			// �J�����O
	if (model != E_MODEL_SKY) { d3dClass->SetZWrite(true); }
	else { d3dClass->SetZWrite(false); }

	for (int i = 0; i < m_pModelData[model]->GetMeshNum(); ++i) {
		ShaderManager::GetInstance().SetTexturePS(m_pModelData[model][i].GetTexture(i));
		m_pModelBuf[model][i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}


void ModelManager::DrawInstanced(E_MODEL model, int num, void* data) {
	//for (int i = 0; i < m_pModelData[model]->GetMeshNum(); ++i) {
	//	ShaderManager::GetInstance().SetTexturePS(m_pModelData[model][i].GetTexture(i));
	//	//m_pModelBuf[model][i].Write(data);
	//	m_pModelBuf[model][i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, num);
	//}
	for (int i = 0; i < m_pModelData[E_MODEL_PLAYER]->GetMeshNum(); ++i) {
		ShaderManager::GetInstance().SetTexturePS(m_pModelData[E_MODEL_PLAYER][i].GetTexture(i));
		//m_pModelBuf[model][i].Write(data);
		m_pShadow[i].WriteInstanceng(data);
		m_pShadow[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, num);
	}
}


FBXPlayer* ModelManager::Get(E_MODEL model) {
	return m_pModelData[model];
}
DrawBuffer* ModelManager::GetBuf(E_MODEL model) {
	return m_pModelBuf[model];
}


// EOF