/**
 * @file WaterSurface.cpp
 */


/**
 * @include
 */
#include "WaterSurface.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Collision.h"
#include "polygon.h"
#include "Light.h"
#include "Shader.h"
#include "System.h"


// �V�F�[�_�ɓn���l
struct SHADER_GLOBAL {
	XMMATRIX	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
	XMMATRIX	mTex;		// �e�N�X�`���s��(�]�u�s��)
};
struct SHADER_GLOBAL2 {
	XMVECTOR	vEye;		// ���_���W
	// ����
	XMVECTOR	vLightDir;	// ��������
	XMVECTOR	vLa;		// �����F(�A���r�G���g)
	XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	XMVECTOR	vLs;		// �����F(�X�y�L����)
	// �}�e���A��
	XMVECTOR	vAmbient;	// �A���r�G���g�F(+�e�N�X�`���L��)
	XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	XMVECTOR	vSpecular;	// �X�y�L�����F(+�X�y�L�������x)
	XMVECTOR	vEmissive;	// �G�~�b�V�u�F
};


/**
 * @constant value
 */
static const DirectX::XMFLOAT4 M_DIFFUSE = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
static const DirectX::XMFLOAT4 M_SPECULAR = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_AMBIENT = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
static const DirectX::XMFLOAT4 M_EMISSIVE = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

static ID3D11Buffer*				g_pConstantBuffer[2];	// �萔�o�b�t�@
static ID3D11SamplerState*			g_pSamplerState;		// �e�N�X�`�� �T���v��
static ID3D11VertexShader*			g_pVertexShader;		// ���_�V�F�[�_
static ID3D11InputLayout*			g_pInputLayout;			// ���_�t�H�[�}�b�g
static ID3D11PixelShader*			g_pPixelShader;			// �s�N�Z���V�F�[�_


WaterSurface::WaterSurface() : GameObject("WaterSurface") {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();

	// �}�e���A���̏����ݒ�
	m_material->m_diffuse = M_DIFFUSE;
	m_material->m_ambient = M_AMBIENT;
	m_material->m_specular = M_SPECULAR;
	m_material->m_power = 50.0f;
	m_material->m_emissive = M_EMISSIVE;

	// �e�N�X�`���ݒ�
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
}


WaterSurface::WaterSurface(std::string name, std::string tag) : GameObject(name, tag) {
	m_transform->m_scale = { 100.f, 100.f, 100.f };
	m_material = AddComponent<Material>();

	// �}�e���A���̏����ݒ�
	m_material->m_diffuse = M_DIFFUSE;
	m_material->m_ambient = M_AMBIENT;
	m_material->m_specular = M_SPECULAR;
	m_material->m_power = 50.0f;
	m_material->m_emissive = M_EMISSIVE;

	// �e�N�X�`���ݒ�
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());
	m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	m_mesh.texSize = float3(1.f, 1.f, 1.f);
	m_mesh.light = true;
	m_mesh.Zbuff = true;
}


WaterSurface::~WaterSurface() {

}


void WaterSurface::Init() {
	// ���b�V���̏�����(�����ݒ�)
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	// �V�F�[�_������
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	LoadShader("MeshVS", "MeshPS", &g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));

	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);

	// �e�N�X�`�� �T���v������
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&sd, &g_pSamplerState);

	// ���_���̍쐬
	m_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
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
	MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

	GameObject::Init();
}


void WaterSurface::Uninit() {
	GameObject::Uninit();
}


void WaterSurface::Update() {
	GameObject::Update();
}


void WaterSurface::LastUpdate() {
	GameObject::LastUpdate();
}


void WaterSurface::Draw() {
	// �e�N�X�`���}�g���b�N�X�̏�����
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;
	mtxTexture = XMMatrixIdentity();
	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(m_mesh.texSize.x, m_mesh.texSize.y, 1.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);
	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_mesh.texSize.x * m_mesh.texPattern.x, m_mesh.texSize.y * m_mesh.texPattern.y, 0.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);
	// �e�N�X�`���}�g���b�N�X�̐ݒ�
	XMStoreFloat4x4(&m_mesh.mtxTexture, mtxTexture);



	//--- �`��
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	Material* pMaterial = new Material();
	MESH* pMesh = &m_mesh;
	Material* material = m_material;
	ID3D11ShaderResourceView* texture = TextureManager::GetInstance().Get(E_TEXTURE::E_TEXTURE_TREE);

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CCW);
	// Z�o�b�t�@����
	D3DClass::GetInstance().SetZBuffer(m_mesh.Zbuff);

	// �V�F�[�_�ݒ�
	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(g_pInputLayout);

	// ���_�o�b�t�@���Z�b�g
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	pDeviceContext->IASetIndexBuffer(pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &texture);

	SHADER_GLOBAL cb;

	XMMATRIX mtxWorld = XMLoadFloat4x4(&m_transform->GetMatrix());
	cb.mWVP = XMMatrixTranspose(mtxWorld *
		XMLoadFloat4x4(&CCamera::Get()->GetView()) * XMLoadFloat4x4(&CCamera::Get()->GetProj()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->mtxTexture));

	pDeviceContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);

	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&CCamera::Get()->m_transform->m_position);
	Light* light = Light::Get();
	cb2.vLightDir =
		pMesh->light ?
		XMVectorSet(light->m_direction.x, light->m_direction.y, light->m_direction.z, 0.f) : XMVectorSet(0.f, 0.f, 0.f, 0.f);
	cb2.vLa = XMLoadFloat4(&light->m_ambient);
	cb2.vLd = XMLoadFloat4(&light->m_diffuse);
	cb2.vLs = XMLoadFloat4(&light->m_specular);

	cb2.vDiffuse = XMLoadFloat4(&material->m_diffuse);
	cb2.vAmbient = XMVectorSet(material->m_ambient.x, material->m_ambient.y, material->m_ambient.z,
		(texture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(material->m_specular.x, material->m_specular.y, material->m_specular.z, material->m_power);
	cb2.vEmissive = XMLoadFloat4(&material->m_emissive);
	pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);

	// �v���~�e�B�u�`����Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���̕`��
	pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);


	//pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);

	// Z�o�b�t�@����
	D3DClass::GetInstance().SetZBuffer(true);

	SAFE_DELETE(pMaterial);
}


// EOF