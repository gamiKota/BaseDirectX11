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
#include "ShaderBufferManager.h"
#include "System.h"


// 静的メンバ
CCamera* CCamera::m_pCamera = nullptr;


void CCamera::Start() {
	m_transform->m_position	= float3(0.0f, 200.0f, -400.0f);
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

	// 上方ベクトル更新
	XMStoreFloat3(&m_transform->m_up,
		XMVector3Normalize(XMLoadFloat3(&m_transform->m_up)));

	// ビュー変換更新
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

	// 射影変換更新
	XMStoreFloat4x4(&m_mProj,
		XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect,
			m_fNearZ, m_fFarZ));

	// カメラのシェーダバッファの更新
	if (this == m_pCamera) {
		SHADER_CAMERA buf;
		XMMATRIX mtxView = XMLoadFloat4x4(&m_mView);
		XMMATRIX mtxProj = XMLoadFloat4x4(&m_mProj);
		buf.vEye = XMLoadFloat3(&m_transform->m_position);
		buf.mV = XMMatrixTranspose(mtxView);
		buf.mP = XMMatrixTranspose(mtxProj);
		ShaderBufferManager::GetInstance().Update("MainCamera", &buf);
		ShaderBufferManager::GetInstance().Bind("MainCamera");
	}
}


// EOF