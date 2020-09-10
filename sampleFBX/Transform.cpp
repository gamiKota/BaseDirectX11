#include "Transform.h"


using namespace DirectX;


Transform::Transform() : m_position(float3()), m_rotate(float3()), m_scale(float3()) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
	// �g�k�̕ύX
	matrix = XMMatrixMultiply(XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z), matrix);
	// ���W�̕ύX
	matrix = XMMatrixMultiply(XMMatrixTranslation(m_position.x, m_position.y, m_position.z), matrix);
	// ��]���̕ύX
	matrix = XMMatrixMultiply(
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(m_rotate.x),
			XMConvertToRadians(m_rotate.y),
			XMConvertToRadians(m_rotate.z)),
		matrix);
	// �s��ɔ��f
	XMStoreFloat4x4(&m_world, matrix);

	// �O�����̍X�V
	m_forward = { m_world._31, m_world._32, m_world._33 };
}

// EOF