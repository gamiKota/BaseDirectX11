#include "Transform.h"
#include <math.h>
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObjectMeshBase.h"
#include "Tween.h"
#include "debugproc.h"
#include "imgui.h"
#include "Camera.h"
#include "System.h"


using namespace DirectX;


// ����
// �N�H�[�^�j�I�����x�N�g���ƂƂ炦��
// �e�v�f(x, y, z)��180������0�`1�`0�ɐ��l�����K��
// �N�H�[�^�j�I����4�̃p�����[�^�̃m�����͂P
// (���̂���w�v�f��-�ɂȂ�ꍇ������)
// �����b�g�Ƃ��āA3x3�̉�]�s��̂����������킸�ɂS�̃p�����[�^�Ɉ��k�������̂��N�H�[�^�j�I��
// �t�ɁA�N�H�[�^�j�I�����^����ꂽ�Ƃ��ɋ�̓I�ȉ�]��p�����C���[�W���ɂ���


Transform::Transform() : m_position(float3()), m_rotation(Quaternion()), m_scale(float3()), m_tween(new Tween()), m_Parent(nullptr) {
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


void Transform::Uninit() {
	delete m_tween;
}


void Transform::Update() {

}


void Transform::LastUpdate() {

	{	// �e�X�gTween
		m_tween->Update();

		if (m_tween->m_isTween == E_TWEEN::DO ||
			m_tween->m_isTween == E_TWEEN::END) {
			m_position = m_tween->GetResult();
			//if (i == 1) {
			//	m_rotate = m_tween[i].GetResult();
			//}
			//if (i == 2) {
			//	m_scale = m_tween[i].GetResult();
			//}
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

	// �N�H�[�^�j�I���͐��K������Ă��邱�Ƃ��O��
	m_rotation = Quaternion::Normalize(m_rotation);
	float3 rotate = Quaternion::RadianAngle(m_rotation);

	// ����
	// ���ɍ폜���ꂽ���ǂ����͍��̏��l���Ȃ�
	// �r���{�[�h���Əo���Ȃ�����
	// �{���ɑS�Ă��e��ɂȂ�
	// Mesh�̏ꍇ�A���W�͐e�̑傫���͈̔͂Ɏw�肳���(0.5�`0.5�͈̔�)
	if (m_Parent != nullptr) {
		XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));

		matrix *= XMLoadFloat4x4(&m_Parent->m_transform->GetMatrix());
		XMStoreFloat4x4(&m_world, matrix);
	}
	else {
		XMMATRIX matrix = XMMatrixIdentity();	// �s��ϊ�
		// �g�k�̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
		// ��]���̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z));
		// ���W�̕ύX
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
		// �s��ɔ��f
		XMStoreFloat4x4(&m_world, matrix);
	}

	// �����x�N�g���̍X�V(�s�b�`�A���[�A���[��)
	// (3x3�̉�]�s��ɂ�����A���K������Ă��ėǂ��̂��͂킩��Ȃ�)
	m_right = float3::Normalize({ m_world._11, m_world._12, m_world._13 });
	m_up = float3::Normalize({ m_world._21, m_world._22, m_world._23 });
	m_forward = float3::Normalize({ m_world._31, m_world._32, m_world._33 });
}


void Transform::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat3("position", (float*)&m_transform->m_position);
	ImGui::DragFloat3("rotation", (float*)&m_transform->m_rotation, 0.01f);
	ImGui::DragFloat3("scale", (float*)&m_transform->m_scale);

	ImGui::DragFloat3("forward", (float*)&m_transform->m_forward);
	ImGui::DragFloat3("up", (float*)&m_transform->m_up);
	ImGui::DragFloat3("right", (float*)&m_transform->m_right);
#endif
}


Tween* Transform::DOMove(float3 position, float time) {

	m_tween->DOTween(m_position, position, time);

	return m_tween;
}


// ����
// Unity�ł͏���x�N�g��������
void Transform::LookAt(Transform* target, float3 worldUp) {
	if (!target)	return;
	m_rotation = Quaternion::LookRotation(target->m_position - m_position, worldUp);
}
void Transform::LookAt(float3 target, float3 worldUp) {
	m_rotation = Quaternion::LookRotation(target - m_position, worldUp);
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