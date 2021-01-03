/**
 * TPCamera.cpp
 */


/**
 * @include
 */
#include "TPCamera.h"
#include "PlayerMgr.h"
#include "PlayerState.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "GameObject3D.h"
#include "input.h"
#include "debugproc.h"
#include "System.h"


// �萔
namespace {
	const float3 g_vEye  = float3(0.0f, 150.0f, -500.0f);	// X-wing
	const float3 g_vLook = float3(0.0f, 120.0f, 0.0f);
	const float3 g_vUp(0.0f, 1.0f, 0.0f);
};


void TPCamera::Awake() {
}


void TPCamera::Start() {
	CCamera::Start();
}


void TPCamera::Uninit() {
	CCamera::Uninit();
}


void TPCamera::Update() {
	CCamera::Update();
}


void TPCamera::LastUpdate() {
	// ����
	// �����_�̓��b�N�I������Ă���^�[�Q�b�g(���b�N�I�����ĂȂ����̓v���C���[)
	// ���W�̓^�[�Q�b�g�ƃv���C���[�̃x�N�g����̏������Ńv���C���[��Y������^���Ɉړ�
	// ���_�ƒ����_���ړ��A����x�N�g������]
	//XMMATRIX world = XMLoadFloat4x4(&m_player->m_transform->GetMatrix());
	//// ���W
	//XMStoreFloat3(&m_vEye, XMVector3TransformCoord(XMLoadFloat3(&g_vEye), world));
	//// �����_
	//XMStoreFloat3(&m_vLook, XMVector3TransformCoord(XMLoadFloat3(&g_vLook), world));
	//// ����x�N�g��
	//XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&g_vUp), world));
	//float3 eye = m_player->m_transform->m_position;
	//eye -= m_player->m_transform->m_forward * 500.f;
	//m_transform->m_position = float3(eye.x, eye.y + 150.f, eye.z);
	//m_vLook = m_player->m_transform->m_position;
	//m_vUp = float3(0.f, 1.f, 0.f);
	//
	//if (m_player->GetComponent<PlayerState>()->GetTarget() != nullptr) {	// �^�[�Q�b�g���b�N�I�����
	//	float3 eye = m_player->m_transform->m_position;
	//	eye -= m_player->m_transform->m_forward * 800.f;
	//	m_transform->m_position = float3(eye.x, eye.y + 150.f, eye.z);
	//	m_vLook = m_player->GetComponent<PlayerState>()->GetTarget()->m_transform->m_position;
	//	m_vUp = float3(0.f, 1.f, 0.f);
	//}
	//�s��X�V
	CCamera::LastUpdate();
}

// EOF