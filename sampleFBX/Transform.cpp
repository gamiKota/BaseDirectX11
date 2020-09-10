#include "Transform.h"


using namespace DirectX;


Transform::Transform() : m_position(float3()), m_rotate(float3()), m_scale(float3()) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
	// 拡縮の変更
	matrix = XMMatrixMultiply(XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z), matrix);
	// 座標の変更
	matrix = XMMatrixMultiply(XMMatrixTranslation(m_position.x, m_position.y, m_position.z), matrix);
	// 回転軸の変更
	matrix = XMMatrixMultiply(
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(m_rotate.x),
			XMConvertToRadians(m_rotate.y),
			XMConvertToRadians(m_rotate.z)),
		matrix);
	// 行列に反映
	XMStoreFloat4x4(&m_world, matrix);

	// 前方向の更新
	m_forward = { m_world._31, m_world._32, m_world._33 };
}

// EOF