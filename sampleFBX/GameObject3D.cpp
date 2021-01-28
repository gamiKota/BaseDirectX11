/**
 * @file GameObject3D.cpp
 */


/**
 * @include
 */
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "D3DClass.h"
#include "Mesh.h"
#include "FbxModel.h"
#include "ShaderManager.h"
#include "System.h"


GameObject3D::GameObject3D(E_MODEL m_model, std::string name, std::string tag) : 
	m_model(m_model), GameObject(name, tag) {
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
	m_material.Ka = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l�̓e�N�X�`��rgb�̓��f�����̂̐F
	m_material.Ke = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l���O�ɂ���Ɛ^���� 
	m_material.Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// �l������������ƃ��f���������Ȃ�
	m_material.Ks = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// ����

	m_isLight = true;
}


GameObject3D::~GameObject3D() {
}


void GameObject3D::Init() {
	GameObject::Init();
	ModelManager::GetInstance().Get(m_model)->SetMaterial(&m_material);
}


void GameObject3D::Uninit() {
	GameObject::Uninit();
}


void GameObject3D::Update() {
	GameObject::Update();
}


void GameObject3D::LastUpdate() {
	GameObject::LastUpdate();
	ModelManager::GetInstance().Update(this);
}


void GameObject3D::Draw() {
	D3DClass* d3dClass = &D3DClass::GetInstance();
	ID3D11Device* pDevice = d3dClass->GetDevice();
	ID3D11DeviceContext* pDeviceContext = d3dClass->GetDeviceContext();

	// �V�F�[�_�̓K�p
	//ShaderManager::GetInstance().UpdateBuffer(obj->m_transform->GetMatrix());
	ShaderManager::GetInstance().BindVS(E_VS_FBX);
	ShaderManager::GetInstance().BindPS(E_PS_FBX);

	// �V�F�[�_�̐ݒ�
	SHADER_LIGHT_SETTING buf;
	buf.light = (m_isLight) ? XMFLOAT4(1.f, 1.f, 1.f, 1.f) : XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	ShaderManager::GetInstance().UpdateBuffer("MainLightSetting", &buf);

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	D3DClass::GetInstance().SetCullMode(CULLMODE_CW);
	D3DClass::GetInstance().SetZWrite(true);
	ModelManager::GetInstance().Draw(this);
	if (GetComponent<Collision>() != nullptr &&
		GetTag() != "Land") {
		GetComponent<Collision>()->DebugDraw();
	}
	//GameObject::Draw();
}


// EOF