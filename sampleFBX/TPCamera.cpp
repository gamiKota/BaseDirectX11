/**
 * TPCamera.cpp
 */


/**
 * @include
 */
#include "TPCamera.h"
#include "Player.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject3D.h"
#include "System.h"


// �萔
namespace {
	const float3 g_vEye  = float3(0.0f, 150.0f, -500.0f);	// X-wing
	const float3 g_vLook = float3(0.0f, 120.0f, 0.0f);
	const float3 g_vUp(0.0f, 1.0f, 0.0f);
};


void TPCamera::Awake() {
	m_vEye = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
	m_fFOVY = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
}


void TPCamera::Start() {
	m_player = GameObject::Find("Player");
	if (m_player == nullptr) {
		MessageBoxA(
			System::GetInstance().GetWnd(),
			"error player nullptr\nload title scene", "TPCamera.cpp",
			MB_OK | MB_ICONERROR | MB_TOPMOST
		);
		SceneManager::GetInstance().LoadScene(E_SCENE::TITLE);
	}
	Update();
}


void TPCamera::Uninit() {
	CCamera::Uninit();
}


void TPCamera::LastUpdate()
{
	// ����
	// �����_�̓��b�N�I������Ă���^�[�Q�b�g(���b�N�I�����ĂȂ����̓v���C���[)
	// ���W�̓^�[�Q�b�g�ƃv���C���[�̃x�N�g����̏������Ńv���C���[��Y������^���Ɉړ�
	// ���_�ƒ����_���ړ��A����x�N�g������]
	XMMATRIX world = XMLoadFloat4x4(&m_player->m_transform->GetMatrix());
	// ���W
	XMStoreFloat3(&m_vEye, XMVector3TransformCoord(XMLoadFloat3(&g_vEye), world));
	// �����_
	XMStoreFloat3(&m_vLook, XMVector3TransformCoord(XMLoadFloat3(&g_vLook), world));
	// ����x�N�g��
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&g_vUp), world));

	if (m_player->GetComponent<Player>()->m_target != nullptr) {	// �^�[�Q�b�g���b�N�I�����
		float3 eye = m_player->m_transform->m_position;
		eye -= m_player->m_transform->m_forward * 600.f;
		m_vEye = float3(eye.x, eye.y + 300.f, eye.z);
		m_vLook = m_player->GetComponent<Player>()->m_target->m_transform->m_position;
		m_vUp = float3(0.f, 1.f, 0.f);
	}

	//�s��X�V
	CCamera::LastUpdate();
}

// EOF