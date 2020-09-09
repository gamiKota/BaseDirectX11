#include "Transform.h"


Transform::Transform() : m_position(DirectX::XMFLOAT3()), m_rotate(XMFLOAT3()), m_scale(XMFLOAT3()) {
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
	// 拡縮の変更
	matrix = XMMatrixMultiply(XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z), matrix);
	// 座標の変更
	matrix = XMMatrixMultiply(XMMatrixTranslation(m_position.x, m_position.y, m_position.z), matrix);
	// 回転軸の変更
	matrix = XMMatrixMultiply(
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotate.x), XMConvertToRadians(m_rotate.y), XMConvertToRadians(m_rotate.z)),
		matrix);
	// 行列に反映
	XMStoreFloat4x4(&m_world, matrix);
}

// EOF