#include "Transform.h"
#include <math.h>


using namespace DirectX;


Transform::Transform() : m_position(float3()), m_rotate(float3()), m_scale(float3()) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// s—ñ•ÏŠ·
	// Šgk‚Ì•ÏX
	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
	// ‰ñ“]Ž²‚Ì•ÏX
	matrix = XMMatrixMultiply(
		matrix,
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(m_rotate.x),
			XMConvertToRadians(m_rotate.y),
			XMConvertToRadians(m_rotate.z)));
	// À•W‚Ì•ÏX
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	// s—ñ‚É”½‰f
	XMStoreFloat4x4(&m_world, matrix);

	// ‘O•ûŒü‚ÌXV
	m_forward = { m_world._31, m_world._32, m_world._33 };
}


void Transform::LookAt(Transform* target) {
	float3 rotate;	// ˆÚ“®æ‰ñ“]Ž²‚ÌŠm•Û
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