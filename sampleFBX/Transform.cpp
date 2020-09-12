#include "Transform.h"
#include <math.h>


using namespace DirectX;


Transform::Transform() : m_position(float3()), m_rotate(float3()), m_scale(float3()) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// 行列変換
	// 拡縮の変更
	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
	// 回転軸の変更
	matrix = XMMatrixMultiply(
		matrix,
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(m_rotate.x),
			XMConvertToRadians(m_rotate.y),
			XMConvertToRadians(m_rotate.z)));
	// 座標の変更
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	// 行列に反映
	XMStoreFloat4x4(&m_world, matrix);

	// 前方向の更新
	m_forward = { m_world._31, m_world._32, m_world._33 };
	// 横方向の更新
	m_right = { m_world._11, m_world._12, m_world._13 };
	// 上方向の更新
	m_up = { m_world._21, m_world._22, m_world._23 };
}


void Transform::LookAt(Transform* target) {
	float3 rotate;	// 移動先回転軸の確保
	rotate.x = XMConvertToDegrees(-atan2f(
		target->m_position.y - m_position.y,
		sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2))));
	rotate.y = XMConvertToDegrees(atan2f(
		target->m_position.x - m_position.x,
		target->m_position.z - m_position.z));
	m_rotate.x = rotate.x;
	m_rotate.y = rotate.y;
}


// EOF