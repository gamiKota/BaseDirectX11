/**
 * @file GameObjectMesh.cpp
 */


/**
 * @include
 */
#include "GameObjectMesh.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Collision.h"
#include "polygon.h"
#include "System.h"


/**
 * @constant value
 */
static const DirectX::XMFLOAT4 M_DIFFUSE	=	XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
static const DirectX::XMFLOAT4 M_SPECULAR	=	XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
static const DirectX::XMFLOAT4 M_AMBIENT	=	XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
static const DirectX::XMFLOAT4 M_EMISSIVE	=	XMFLOAT4(0.0f,0.0f,0.0f,1.0f);


GameObjectMesh::GameObjectMesh(E_MESH_TYPE mesh) : m_type(mesh), m_texture(E_TEXTURE_NONE), GameObject("GameObjectMesh") {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();
}


GameObjectMesh::GameObjectMesh(E_MESH_TYPE mesh, E_TEXTURE texture, std::string name, std::string tag) :
	m_type(mesh), m_texture(texture), GameObject(name, tag) {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();
}


GameObjectMesh::~GameObjectMesh() {
}


void GameObjectMesh::Init() {
	// メッシュの初期化(初期設定)
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;
	// マテリアルの初期設定
	m_material->m_diffuse	= M_DIFFUSE;
	m_material->m_ambient	= M_AMBIENT;
	m_material->m_specular	= M_SPECULAR;
	m_material->m_power		= 50.0f;
	m_material->m_emissive	= M_EMISSIVE;

	// テクスチャ設定
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	// ワールドマトリックス初期化
	//XMStoreFloat4x4(&m_mesh.mtxWorld, XMMatrixIdentity());

	// 頂点情報の作成
	m_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-0.5f,  0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3( 0.5f,  0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3( 0.5f, -0.5f, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	m_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	hr = MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

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
		DrawMesh(&m_mesh, m_material, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrixBillboard());
	}
	else {
		DrawMesh(&m_mesh, m_material, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrix());
	}
}


// EOF