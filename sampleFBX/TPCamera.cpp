// �O�l�̃J���� [TPCamera.cpp]
#include "TPCamera.h"
#include "Player.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject.h"
#include "System.h"


// �萔
namespace {
	const XMFLOAT3 g_vEye = XMFLOAT3(0.0f, 150.0f, -500.0f);	// X-wing
	const XMFLOAT3 g_vLook = XMFLOAT3(0.0f, 120.0f, 0.0f);
	const XMFLOAT3 g_vUp(0.0f, 1.0f, 0.0f);
};

// ������
void TPCamera::Awake()
{
	m_vEye = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
	m_fFOVY = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
	Update();
}

void TPCamera::Start()
{
	m_vEye = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
	m_fFOVY = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
	Update();
}

// �I������
void TPCamera::Uninit()
{
	CCamera::Uninit();
}

// �X�V
void TPCamera::Update()
{
	// ���_�ƒ����_���ړ��A����x�N�g������]
	GameObject* player = GameObject::Find("Player");
	if (player != nullptr) {
		XMMATRIX world = XMLoadFloat4x4(&player->m_transform->GetMatrix());
		XMStoreFloat3(&m_vEye, XMVector3TransformCoord(
			XMLoadFloat3(&g_vEye), world));
		XMStoreFloat3(&m_vLook, XMVector3TransformCoord(
			XMLoadFloat3(&g_vLook), world));
		XMStoreFloat3(&m_vUp, XMVector3TransformNormal(
			XMLoadFloat3(&g_vUp), world));
	}
	//�s��X�V
	CCamera::Update();
}
