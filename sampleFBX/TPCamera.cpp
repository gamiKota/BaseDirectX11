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


void TPCamera::Awake() {
}


void TPCamera::Start() {
	CCamera::Start();
	m_player = GameObject::Find("Player")->GetComponent<PlayerMgr>();
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

	// �Ǐ]����
	// �v���C���[�̉��ړ��ł��炵����
	if (m_player != nullptr) {
		GameObject* target = m_player->m_state->GetTarget();
		// ���f���p���Ɉˑ����Ȃ����s�ړ�
		XMFLOAT4X4 mtx = XMFLOAT4X4();
		XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(m_player->m_transform->m_rotate.x, m_player->m_transform->m_rotate.y, 0.f));
		float3 right = float3(mtx._11, mtx._12, mtx._13);
		float3 up = float3(mtx._21, mtx._22, mtx._23);
		float3 forward = float3(mtx._31, mtx._32, mtx._33);
		if (target != nullptr) {	// �^�[�Q�b�g�I��
			m_transform->LookAt(target->m_transform);
			SetLook(target->m_transform);
		}
		else {	// �^�[�Q�b�g�I�t
			m_transform->LookAt(m_player->m_transform);
			SetLook(nullptr);
		}
		m_transform->m_position = m_player->m_transform->m_position;
		m_transform->m_position -= forward * 250.f;
		m_transform->m_position += up * 150.f;
	}

	//�s��X�V
	CCamera::LastUpdate();
}

// EOF