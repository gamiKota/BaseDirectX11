#include "Transform.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObjectMeshBase.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "Camera.h"
#include "System.h"


using namespace DirectX;


float normalize(float num, float min = 0, float max = 1) {
	float range = abs(max - min);
	if (num < min) {
		return num + range * ceil(abs(num - min) / range);
	}
	else if(max <= num) {
		return num - range * (floor(abs(num - max) / range) + 1);
	}

	return num;
}



Transform::Transform() : m_position(float3()), m_rotate(Quaternion()), m_scale(float3()), m_tween(new Tween[3]()), m_Parent(nullptr) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::Uninit() {
	delete[] m_tween;
}


void Transform::Update() {

}


void Transform::LastUpdate() {

	for (int i = 0; i < 3; i++) {
		m_tween[i].Update();

		if (m_tween[i].m_isTween == E_TWEEN::DO ||
			m_tween[i].m_isTween == E_TWEEN::END) {	// ����
			if (i == 0) {
				m_position = m_tween[i].GetResult();
			}
			if (i == 1) {
				m_rotate = m_tween[i].GetResult();
			}
			if (i == 2) {
				m_scale = m_tween[i].GetResult();
			}
		}
	}

	// ��荇���������
	if (dynamic_cast<GameObjectUI*>(m_gameObject) != nullptr) {
		m_transform->m_position.z = 0;
		m_transform->m_scale.z = 0;
	}
	if (dynamic_cast<GameObjectMeshBase*>(m_gameObject) != nullptr) {
		m_transform->m_scale.z = 0;
	}

	m_rotate.x = normalize(m_rotate.x, -XM_PI, XM_PI);
	m_rotate.y = normalize(m_rotate.y, -XM_PI, XM_PI);
	//m_rotate.z = normalize(m_rotate.z, -XM_PI, XM_PI);

	// ����
	// ���ɍ폜���ꂽ���ǂ����͍��̏��l���Ȃ�
	// �r���{�[�h���Əo���Ȃ�����
	// �{���ɑS�Ă��e��ɂȂ�
	// Mesh�̏ꍇ�A���W�͐e�̑傫���͈̔͂Ɏw�肳���(0.5�`0.5�͈̔�)
	if (m_Parent != nullptr) {
		XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));

		matrix *= XMLoadFloat4x4(&m_Parent->m_transform->GetMatrix());
		XMStoreFloat4x4(&m_world, matrix);
	}
	else {
		XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
		// �g�k�̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		// ��]���̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z));
		// ���W�̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
		// �s��ɔ��f
		XMStoreFloat4x4(&m_world, matrix);
	}

	// �O�����̍X�V
	m_forward = { m_world._31, m_world._32, m_world._33 };
	//m_forward = float3::Normalize(m_forward);
	// �������̍X�V
	m_right = { m_world._11, m_world._12, m_world._13 };
	//m_right = float3::Normalize(m_right);
	// ������̍X�V
	m_up = { m_world._21, m_world._22, m_world._23 };
	//m_up = float3::Normalize(m_up);
}


void Transform::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat3("position", (float*)&m_transform->m_position);
	ImGui::DragFloat3("rotation", (float*)&m_transform->m_rotate, 0.01f);
	ImGui::DragFloat3("scale", (float*)&m_transform->m_scale);

	ImGui::DragFloat3("forward", (float*)&m_transform->m_forward);
	ImGui::DragFloat3("up", (float*)&m_transform->m_up);
	ImGui::DragFloat3("right", (float*)&m_transform->m_right);
#endif
}


Tween* Transform::DOMove(float3 position, float time) {

	m_tween[0].DOTween(m_position, position, time);

	return &m_tween[0];
}


void Transform::LookAt(Transform* target, float angle) {
	if (!target)	return;
	float3 rotate = float3();	// �ړ����]���̊m��
	rotate.x = -atan2f(
		target->m_position.y - m_position.y,
		sqrtf(powf(target->m_position.z - m_position.z, 2) + powf(target->m_position.x - m_position.x, 2)));
	rotate.y = atan2f(
		target->m_position.x - m_position.x,
		target->m_position.z - m_position.z);

	
	float3 diffAngle = rotate - m_rotate;
	float3 unti = float3(1.f, 1.f, 1.f);

	if (diffAngle.x < -XM_PI) {
		unti.x = -1.f;
		diffAngle.x = (m_rotate.x - rotate.x) - XM_2PI;
	}
	else if (diffAngle.x > XM_PI) {
		unti.x = -1.f;
		diffAngle.x = (m_rotate.x - rotate.x) + XM_2PI;
	}
	if (diffAngle.y < -XM_PI) {
		unti.y = -1.f;
		diffAngle.y = (m_rotate.y - rotate.y) - XM_2PI;
	}
	else if (diffAngle.y > XM_PI) {
		unti.y = -1.f;
		diffAngle.y = (m_rotate.y - rotate.y) + XM_2PI;
	}

	// �p�x����
	if (angle > 0.f && angle < XM_2PI) {
		if (diffAngle.x > angle) {
			diffAngle.x = angle;
		}
		else if (diffAngle.x < -angle) {
			diffAngle.x = -angle;
		}
		if (diffAngle.y > angle) {
			diffAngle.y = angle;
		}
		else if (diffAngle.y < -angle) {
			diffAngle.y = -angle;
		}
	}

	m_rotate.x += diffAngle.x * unti.x;
	m_rotate.y += diffAngle.y * unti.y;
}


XMFLOAT4X4 Transform::GetMatrixBillboard() {

	XMFLOAT4X4& mView = CCamera::Get()->GetView();

	XMFLOAT4X4 mtxWorld;
	XMStoreFloat4x4(&mtxWorld, XMMatrixIdentity());

	mtxWorld._11 = mView._11 * m_scale.x;
	mtxWorld._12 = mView._21 * m_scale.x;
	mtxWorld._13 = mView._31 * m_scale.x;
	mtxWorld._21 = mView._12 * m_scale.y;
	mtxWorld._22 = mView._22 * m_scale.y;
	mtxWorld._23 = mView._32 * m_scale.y;
	mtxWorld._31 = mView._13 * m_scale.z;
	mtxWorld._32 = mView._23 * m_scale.z;
	mtxWorld._33 = mView._33 * m_scale.z;
	// �ʒu�𔽉f
	mtxWorld._41 = m_position.x;
	mtxWorld._42 = m_position.y;
	mtxWorld._43 = m_position.z;

	return mtxWorld;
}


// EOF