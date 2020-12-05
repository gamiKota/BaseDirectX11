/**
 * @file GameObjectMesh.cpp
 */


/**
 * @include
 */
#include "GameObjectMesh.h"
#include "D3DClass.h"
#include "Camera.h"
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
	TextureManager::GetInstance().Load(m_texture);

	// ���b�V���̏�����(�����ݒ�)
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;
	// �}�e���A���̏����ݒ�
	m_material->m_diffuse	= M_DIFFUSE;
	m_material->m_ambient	= M_AMBIENT;
	m_material->m_specular	= M_SPECULAR;
	m_material->m_power		= 50.0f;
	m_material->m_emissive	= M_EMISSIVE;


	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());

	// ���[���h�}�g���b�N�X������
	//XMStoreFloat4x4(&m_mesh.mtxWorld, XMMatrixIdentity());

	// ���_���̍쐬
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
	TextureManager::GetInstance().Release(m_texture);
}


void GameObjectMesh::Update() {
	GameObject::Update();
}


void GameObjectMesh::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectMesh::Draw() {
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);

	// �`��
	if (m_type == E_MESH_TYPE::BILLBORAD) {
		DrawMesh(&m_mesh, m_material, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrixBillboard());
	}
	else {
		DrawMesh(&m_mesh, m_material, TextureManager::GetInstance().Get(m_texture), &m_transform->GetMatrix());
	}

	D3DClass::GetInstance().SetBlendState(BS_NONE);
}


// EOF