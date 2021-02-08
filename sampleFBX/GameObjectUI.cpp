/**
 * @file GameObjectUI.cpp
 */


/**
 * @include
 */
#include "GameObjectUI.h"
#include "D3DClass.h"
#include "polygon.h"
#include "Geometory.h"
#include "Material.h"
#include "Text.h"
#include "System.h"


GameObjectUI::GameObjectUI(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	m_layer(layer), m_texture(texture), GameObject(name, tag) {
	// �ϐ��̏�����
	m_transform->m_scale = { 100.f, 100.f, 0 };
	m_text = new Text;
	// �e�N�X�`���ݒ�
	m_texPattern = float3(0.f, 0.f, 0.f);
	m_texSize = float3(1.f, 1.f, 1.f);
	// �}�e���A��
	m_material = AddComponent<Material>();
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l�̓e�N�X�`��rgb�̓��f�����̂̐F
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l���O�ɂ���Ɛ^���� 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// �l������������ƃ��f���������Ȃ�
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// ����
	// �V�F�[�_
	m_vs = VS_2D;
	m_ps = PS_2D;
	// �e�L�X�g
	m_text->SetFontSize(16.f);
}


GameObjectUI::~GameObjectUI() {}


void GameObjectUI::Init() {
	GameObject::Init();
}


void GameObjectUI::Uninit() {
	GameObject::Uninit();
	delete m_text;
}


void GameObjectUI::Update() {
	GameObject::Update();
}


void GameObjectUI::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectUI::Draw() {

	ShaderManager* shader = &ShaderManager::GetInstance();
	ID3D11DeviceContext* DC = D3DClass::GetInstance().GetDeviceContext();

	shader->BindVS(m_vs);
	shader->BindPS(m_ps);
	DC->HSSetShader(NULL, NULL, 0);
	DC->DSSetShader(NULL, NULL, 0);
	DC->GSSetShader(NULL, NULL, 0);
	DC->CSSetShader(NULL, NULL, 0);

	// �e�N�X�`���}�g���b�N�X�̏�����
	XMMATRIX mtxTex, mtxScale, mtxTranslate;
	mtxTex = XMMatrixIdentity();
	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(m_texSize.x, m_texSize.y, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex, mtxScale);
	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_texSize.x * m_texPattern.x, m_texSize.y * m_texPattern.y, 0.0f);
	mtxTex = XMMatrixMultiply(mtxTex, mtxTranslate);

	// �s����
	SHADER_WORLD world;
	world.mWorld	= XMMatrixTranspose(XMLoadFloat4x4(&m_transform->GetMatrix()));
	world.mTexture	= XMMatrixTranspose(mtxTex);
	shader->UpdateBuffer("MainWorld", &world);
	// �}�e���A��
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// �e�N�X�`���̔��f
	shader->SetTexturePS(TextureManager::GetInstance().Get(m_texture));

	// �`��
	DrawPolygon();
	GameObject::Draw();

	// �e�L�X�g�\��
	shader->BindVS(E_VS::VS_NORMAL);
	shader->BindPS(E_PS::PS_NORMAL);
	m_text->Bind();
}


bool GameObjectUISort(GameObject* obj1, GameObject* obj2) {
	GameObjectUI* temp1 = dynamic_cast<GameObjectUI*>(obj1);
	GameObjectUI* temp2 = dynamic_cast<GameObjectUI*>(obj2);
	if (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->m_layer < temp2->m_layer) {
			return true;
		}
	}
	return false;
}


// EOF