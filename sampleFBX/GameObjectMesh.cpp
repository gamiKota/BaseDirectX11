/**
 * @file GameObjectMesh.cpp
 */


/**
 * @include
 */
#include "GameObjectMesh.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "Material.h"
#include "System.h"



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

	// マテリアル
	m_material = AddComponent<Material>();
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値はテクスチャrgbはモデル自体の色
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値を０にすると真っ白 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 値を小さくするとモデルが薄くなる
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// 光沢
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
	shader->BindVS(VS_MESH);
	shader->BindPS(PS_2D);
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

	// マテリアル
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// 描画
	if (m_type == E_MESH_TYPE::BILLBORAD) {
		DrawMesh(&m_mesh, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrixBillboard());
	}
	else {
		DrawMesh(&m_mesh, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrix());
	}
}


// EOF