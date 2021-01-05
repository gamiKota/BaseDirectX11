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
	m_fFarZ = 25000.0f;
	m_player = GameObject::Find("Player")->GetComponent<PlayerMgr>();
}


void TPCamera::Uninit() {
	CCamera::Uninit();
}


void TPCamera::Update() {

	// �Ǐ]����
	// �v���C���[�̉��ړ��ł��炵����
	// �������̂����Ȃ̂��K�N��
	if (m_player != nullptr) {
		GameObject* target = m_player->m_state->GetTarget();
		// ���f���p���Ɉˑ����Ȃ����s�ړ�
		XMFLOAT4X4 mtx = XMFLOAT4X4();
		float3 rotate = Quaternion::RadianAngle(m_player->m_transform->m_rotation);
		XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, 0.f));
		float3 right = float3(mtx._11, mtx._12, mtx._13);
		float3 up = float3(mtx._21, mtx._22, mtx._23);
		float3 forward = float3(mtx._31, mtx._32, mtx._33);
		if (target != nullptr) {	// �^�[�Q�b�g�I��
			//m_transform->LookAt(target->m_transform);
			m_transform->LookAt(target->m_transform, m_player->m_transform->m_up);
			//SetLook(target->m_transform);
		}
		else {	// �^�[�Q�b�g�I�t
			m_transform->LookAt(m_player->m_transform);
			//SetLook(nullptr);
		}
		m_transform->m_position = m_player->m_transform->m_position;
		m_transform->m_position -= forward * 500.f;
		m_transform->m_position += up * 150.f;
		//CCamera::Get()->SetLook(m_player->m_transform);
	}

	CCamera::Update();
}


void TPCamera::LastUpdate() {
	// ����
	// �����_�̓��b�N�I������Ă���^�[�Q�b�g(���b�N�I�����ĂȂ����̓v���C���[)
	// ���W�̓^�[�Q�b�g�ƃv���C���[�̃x�N�g����̏������Ńv���C���[��Y������^���Ɉړ�
	// ���_�ƒ����_���ړ��A����x�N�g������]

	//�s��X�V
	CCamera::LastUpdate();
}

// EOF