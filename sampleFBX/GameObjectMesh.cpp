/**
 * @file GameObjectMesh.cpp
 */


/**
 * @include
 */
#include "GameObjectMesh.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "System.h"


GameObjectMesh::GameObjectMesh(E_MESH_TYPE mesh) : m_type(mesh), m_texture(E_TEXTURE_NONE), GameObjectMeshBase("GameObjectMesh") {
	m_transform->m_scale = { 100.f, 100.f, 100.f };

	// テクスチャ設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
	m_mesh.isDraw = true;
}


GameObjectMesh::GameObjectMesh(E_MESH_TYPE mesh, E_TEXTURE texture, std::string name, std::string tag) :
	m_type(mesh), m_texture(texture), GameObjectMeshBase(name, tag) {
	m_transform->m_scale = { 100.f, 100.f, 100.f };

	// テクスチャ設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
	m_mesh.isDraw = true;
}


GameObjectMesh::~GameObjectMesh() {
}


void GameObjectMesh::Init() {
	GameObject::Init();
}


void GameObjectMesh::Uninit() {
	GameObject::Uninit();
}


void GameObjectMesh::Update() {
	GameObject::Update();
}


void GameObjectMesh::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectMesh::Draw() {

	D3DClass::GetInstance().SetCullMode(CULLMODE_NONE);
	D3DClass::GetInstance().SetZBuffer(true);
	D3DClass::GetInstance().SetBlendState(EBlendState::BS_ALPHABLEND);

	ShaderManager* shader = &ShaderManager::GetInstance();
	shader->BindPS(PS_2D);
	shader->BindVS(VS_MESH);
	shader->BindGS(GS_NORMAL);

	shader->SetTexturePS(TextureManager::GetInstance().Get(m_texture));

	// テクスチャマトリックスの初期化
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;
	mtxTexture = XMMatrixIdentity();
	// スケールを反映
	mtxScale = XMMatrixScaling(m_mesh.texSize.x, m_mesh.texSize.y, 1.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);
	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_mesh.texSize.x * m_mesh.texPattern.x, m_mesh.texSize.y * m_mesh.texPattern.y, 0.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);
	// テクスチャマトリックスの設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, mtxTexture);


	// 描画
	if (m_type == E_MESH_TYPE::BILLBORAD) {
		DrawMesh(&m_mesh, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrixBillboard());
	}
	else {
		DrawMesh(&m_mesh, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrix());
	}
}


// EOF