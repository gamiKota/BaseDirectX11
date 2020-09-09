#include "Transform.h"


Transform::Transform() : m_position(DirectX::XMFLOAT3()), m_rotate(XMFLOAT3()), m_scale(XMFLOAT3()) {
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
	// �g�k�̕ύX
	matrix = XMMatrixMultiply(XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z), matrix);
	// ���W�̕ύX
	matrix = XMMatrixMultiply(XMMatrixTranslation(m_position.x, m_position.y, m_position.z), matrix);
	// ��]���̕ύX
	matrix = XMMatrixMultiply(
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotate.x), XMConvertToRadians(m_rotate.y), XMConvertToRadians(m_rotate.z)),
		matrix);
	// �s��ɔ��f
	XMStoreFloat4x4(&m_world, matrix);
}

// EOF