#include "Transform.h"
#include "GameObject.h"
#include "debugproc.h"
#include "System.h"


using namespace DirectX;


float lerp(float3 start, float3 end, float t) {
	return ((1 - t) * start.y + t * end.y);
}


Transform::Transform() : m_position(float3()), m_rotate(Quaternion()), m_scale(float3()) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::Update() {
	//XMQuaternionSlerp
}


void Transform::LastUpdate() {
	XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
	// �g�k�̕ύX
	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
	// ��]���̕ύX
	matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z));
	// ���W�̕ύX
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	// �s��ɔ��f
	XMStoreFloat4x4(&m_world, matrix);

	// �O�����̍X�V
	m_forward = { m_world._31, m_world._32, m_world._33 };
	// �������̍X�V
	m_right = { m_world._11, m_world._12, m_world._13 };
	// ������̍X�V
	m_up = { m_world._21, m_world._22, m_world._23 };
}


void Transform::LookAt(Transform* target) {
	float3 rotate = float3();	// �ړ����]���̊m��
	rotate.x = -atan2f(
		target->m_position.y - m_position.y,
		sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2)));
	rotate.y = atan2f(
		target->m_position.x - m_position.x,
		target->m_position.z - m_position.z);

	m_rotate.x = rotate.x;
	m_rotate.y = rotate.y;
}


void Transform::LookAtA(Transform* target) {
	//float3 rotate = float3();	// �ړ����]���̊m��
	//rotate.x = XMConvertToDegrees(-atan2f(
	//	target->m_position.y - m_position.y,
	//	sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2))));
	//rotate.y = XMConvertToDegrees(atan2f(
	//	target->m_position.x - m_position.x,
	//	target->m_position.z - m_position.z));
	//rotate.z = m_rotate.z;
	//
	//m_DORotate.start	= m_rotate;
	//m_DORotate.end		= rotate;
	//m_DORotate.time		= 5;

	//PrintDebugProc("LookAtA = %.2f, %.2f, %.2f\n", rotate.x, rotate.y, rotate.z);
	//float3 rot = float3(rotate.x - m_rotate.x, rotate.y - m_rotate.y, rotate.z - m_rotate.z);
	//PrintDebugProc("LookAtA = %.2f, %.2f, %.2f\n", rot.x, rot.y, rot.z);
	//// �����J�[�u�ɂ�����...
	//if (abs(rot.x) <= 10.f && abs(rot.y) <= 10.f && abs(rot.z) <= 10.f) {
	//	rot *= 0.1f;
	//}
	//else {
	//	rot *= 0.01f;
	//}
	//m_rotate.x += rot.x;
	//m_rotate.y += rot.y;
}


// EOF