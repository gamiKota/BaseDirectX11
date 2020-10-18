// ��l�̃J���� [FPCamera.cpp]
#include "FPCamera.h"
#include "Player.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject.h"
#include "System.h"

// �萔
namespace {
	const float3 g_vEye  = float3(0.0f, 14.69f, 24.42f);	// X-wing
	const float3 g_vLook = float3(0.0f, 14.69f, 124.42f);	// X-wing
	const float3 g_vUp(0.0f, 1.0f, 0.0f);
};

// ������
void FPCamera::Awake()
{
	m_vEye = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
	m_fFovy = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
	Update();
}

void FPCamera::Start() {

}

// �I������
void FPCamera::Uninit()
{
	CCamera::Uninit();
}

// �X�V
void FPCamera::Update()
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
	// �s��X�V
	CCamera::Update();
}
