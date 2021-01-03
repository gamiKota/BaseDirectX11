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
#include "System.h"

// 静的メンバ
CCamera* CCamera::m_pCamera = nullptr;
float3 CCamera::m_vNowEye;		// 現在の視点
float3 CCamera::m_vNowLook;		// 現在の注視点
float3 CCamera::m_vNowUp;		// 現在の上方ベクトル


void CCamera::Start() {
	m_transform->m_position	= float3(0.0f, 200.0f, -400.0f);
	//m_transform->m_rotate = float3(0.f, 0.f, 0.f);
	m_transform->m_scale = float3(1.f, 1.f, 1.f);
	//m_vLook = float3(0.0f, 0.0f, 0.0f);
	//m_vUp	= float3(0.0f, 1.0f, 0.0f);
	m_fFovy = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 10000.0f;
	m_vNowEye = m_transform->m_position;
	//m_vNowLook = m_vLook;
	//m_vNowUp = m_vUp;
	Update();
}


void CCamera::Uninit() {
	
}

void CCamera::Update() {

}


void CCamera::LastUpdate() {
	// 視点、注視点、上方ベクトルを近づける
	//m_vNowEye.x = m_vNowEye.x * 0.5f + m_vEye.x * 0.5f;
	//m_vNowEye.y = m_vNowEye.y * 0.5f + m_vEye.y * 0.5f;
	//m_vNowEye.z = m_vNowEye.z * 0.5f + m_vEye.z * 0.5f;
	//m_vNowLook.x = m_vNowLook.x * 0.5f + m_vLook.x * 0.5f;
	//m_vNowLook.y = m_vNowLook.y * 0.5f + m_vLook.y * 0.5f;
	//m_vNowLook.z = m_vNowLook.z * 0.5f + m_vLook.z * 0.5f;
	//m_vNowUp.x = m_vNowUp.x * 0.5f + m_vUp.x * 0.5f;
	//m_vNowUp.y = m_vNowUp.y * 0.5f + m_vUp.y * 0.5f;
	//m_vNowUp.z = m_vNowUp.z * 0.5f + m_vUp.z * 0.5f;

	// 上方ベクトル更新
	XMStoreFloat3(&m_transform->m_up,
		XMVector3Normalize(XMLoadFloat3(&m_transform->m_up)));

	// ビュー変換更新
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_transform->m_position),
		XMLoadFloat3(&(m_transform->m_position + m_transform->m_forward * CAMERA_LOOK_LENG)),
		XMLoadFloat3(&m_transform->m_up)));

	// 射影変換更新
	XMStoreFloat4x4(&m_mProj,
		XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect,
			m_fNearZ, m_fFarZ));
}


// EOF