/**
 * @file Camera.cpp
 */


/**
 * @include
 */
#include "Camera.h"
#include "D3DClass.h"
#include "Graphics.h"
#include "debugproc.h"
#include "Transform.h"
#include "ShaderManager.h"
#include "System.h"


// �ÓI�����o
CCamera* CCamera::m_pCamera = nullptr;


void CCamera::Start() {
	//m_transform->m_position	= float3(0.0f, 200.0f, -400.0f);
	m_transform->m_scale = float3(0.1f, 0.1f, 0.1f);

	m_fFovy = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;

	m_lookTarget = nullptr;
}


void CCamera::Uninit() {
	
}

void CCamera::Update() {

}


void CCamera::LastUpdate() {

	// ����x�N�g���X�V
	XMStoreFloat3(&m_transform->m_up,
		XMVector3Normalize(XMLoadFloat3(&m_transform->m_up)));

	// �r���[�ϊ��X�V
	float3 look = float3();
	if (m_lookTarget != nullptr) {
		look = m_lookTarget->m_position;
	}
	else {
		look = m_transform->m_position + m_transform->m_forward * CAMERA_LOOK_LENG;
	}
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_transform->m_position),
		XMLoadFloat3(&look),
		XMLoadFloat3(&m_transform->m_up)));

	// �ˉe�ϊ��X�V
	XMStoreFloat4x4(&m_mProj,
		XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect,
			m_fNearZ, m_fFarZ));

	// �J�����̃V�F�[�_�o�b�t�@�̍X�V
	if (this == m_pCamera) {
		SHADER_CAMERA buf;
		XMMATRIX mtxView = XMLoadFloat4x4(&m_mView);
		XMMATRIX mtxProj = XMLoadFloat4x4(&m_mProj);
		buf.vEye = XMLoadFloat3(&m_transform->m_position);
		buf.mV = XMMatrixTranspose(mtxView);
		buf.mP = XMMatrixTranspose(mtxProj);
		buf.mV2D = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		buf.mP2D = XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f);

		ShaderManager::GetInstance().UpdateBuffer("MainCamera", &buf);
	}
}


// EOF