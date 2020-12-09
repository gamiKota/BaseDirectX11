/**
 * @file TargetCtr.cpp
 */


/**
 * @include
 */
#include "TargetCtr.h"
#include <DirectXMath.h>
#include "Graphics.h"
#include "debugproc.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "Player.h"
#include "System.h"


using namespace DirectX;


//�x�N�g���̒������v�Z����
float get_vector_length(float3 v) {
	return powf((v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5);
}

//�x�N�g������
float dot_product(float3 vl, float3 vr) {
	return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
}


float3 LockOnMarker(Transform* target) {
	float3 OutPos = float3();
	
	// �J�����̌����Ă�x�N�g����
	// �Ώە��̃J�����ɑ΂��Ẵx�N�g�������ƕ���
	// �J�����ɑ΂��Ă̌�딻�肪�Ƃꂻ��
	float3 CameraVec = GameObject::Find("Player")->m_transform->m_forward;
	float3 TargetVec = float3::Normalize(CCamera::Get()->m_transform->m_position - target->m_transform->m_position);
	// ��̃x�N�g���̒������擾
	float length_A = get_vector_length(CameraVec);
	float length_B = get_vector_length(TargetVec);
	//���ςƃx�N�g���������g����cos�Ƃ����߂�
	float cos_sita = dot_product(CameraVec, TargetVec) / (length_A * length_B);
	//cos�Ƃ���Ƃ����߂�
	float sita = acos(cos_sita);
	//���W�A���łȂ�0�`180�̊p�x�łق����ꍇ�̓R�����g�O��
	sita = sita * 180.f / XM_PI;
	
	PrintDebugProc("sita = %.2f\n", sita);
	if (sita < 90.f) {
		return float3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);
	}


	// �r���[�s��Ǝˉe�s��̎擾
	XMMATRIX view = XMLoadFloat4x4(&CCamera().Get()->GetView());
	XMMATRIX proj = XMLoadFloat4x4(&CCamera().Get()->GetProj());

	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	float w = (float)SCREEN_CENTER_X;
	float h = (float)SCREEN_CENTER_Y;

	XMMATRIX viewport = XMLoadFloat4x4(&XMFLOAT4X4(
		w,  0, 0, 0,
		0, -h, 0, 0,
		0,  0, 1, 0,
		w,  h, 0, 1
	));

	XMVECTOR vScreenPos;
	XMVECTOR vViewProj = XMLoadFloat3(&target->m_position);

	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	vViewProj = XMVector3Transform(vViewProj, view);
	vViewProj = XMVector3Transform(vViewProj, proj);

	// z�Ŋ�����-1~1�͈̔͂Ɏ��߂�(���K��)
	XMFLOAT3 norPos;
	XMStoreFloat3(&norPos, vViewProj);
	norPos.x /= norPos.z; norPos.y /= norPos.z; norPos.z /= norPos.z;
	XMVECTOR vec = XMLoadFloat3(&norPos);
	// �X�N���[���ϊ�
	vScreenPos = XMVector3Transform(vec, viewport);

	XMFLOAT3 ScreenPos;
	XMStoreFloat3(&ScreenPos, vScreenPos);

	OutPos.x =  ScreenPos.x - SCREEN_CENTER_X;
	OutPos.y = -ScreenPos.y + SCREEN_CENTER_Y;
	OutPos.z =  ScreenPos.z;

	return OutPos;
}


void TargetCtr::Start() {
	// �ϐ�������
	m_transform->m_scale = float3(100.f, 100.f, 0.f);
	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	obj->m_shader = E_SHADER_2D::_COLOR;

	if (m_target == nullptr) {
		m_target = GameObject::FindGameObjectWithTag("Enemy");
	}
}


void TargetCtr::Update() {
	// ���b�N�I���}�[�J�[
	m_transform->m_position = LockOnMarker(m_target->m_transform);

	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	// ��ʊO
	// �������΂�������
	// �����܂ł����W��3D�I�u�W�F�N�g��ǂ��Ă邩��X�N���[�����W�֕ϊ��������̒l���]��ł���̂Ƃ͌���Ȃ�
	// ��荇���������R�[�h�Ŏ���
	if (m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X ||
		m_transform->m_position.x - m_transform->m_scale.x * 0.5f >  (float)SCREEN_CENTER_X	||
		m_transform->m_position.y - m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y ||
		m_transform->m_position.y + m_transform->m_scale.y * 0.5f >  (float)SCREEN_CENTER_Y) {

		float angle = atan2(GameObject::Find("Player")->m_transform->m_position.z - m_target->m_transform->m_position.z,
			GameObject::Find("Player")->m_transform->m_position.x - m_target->m_transform->m_position.x);

		m_transform->m_position.x = 0.f - cosf(angle + GameObject::Find("Player")->m_transform->m_rotate.y) * SCREEN_CENTER_X;
		m_transform->m_position.y = 0.f - sinf(angle + GameObject::Find("Player")->m_transform->m_rotate.y) * SCREEN_CENTER_X;

		if (m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X) {
			m_transform->m_position.x = -(float)SCREEN_CENTER_X + m_transform->m_scale.x * 0.5f;
		}
		if (m_transform->m_position.x - m_transform->m_scale.x * 0.5f > (float)SCREEN_CENTER_X) {
			m_transform->m_position.x = (float)SCREEN_CENTER_X - m_transform->m_scale.x * 0.5f;
		}
		if (m_transform->m_position.y - m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y) {
			m_transform->m_position.y = -(float)SCREEN_CENTER_Y + m_transform->m_scale.y * 0.5f;
		}
		if (m_transform->m_position.y + m_transform->m_scale.y * 0.5f > (float)SCREEN_CENTER_Y) {
			m_transform->m_position.y = (float)SCREEN_CENTER_Y - m_transform->m_scale.y * 0.5f;
		}
		obj->m_texture = E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI;
		obj->m_color = float3(1.f, 0.f, 0.f);
		obj->m_layer = E_LAYER::UI;
		m_transform->m_rotate.z = atan2(m_transform->m_position.y, m_transform->m_position.x) * 180.f / XM_PI;
	}
	// ��ʓ�
	else {
		if (m_target == GameObject::Find("Player")->GetComponent<PlayerCtr>()->m_target) {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_MAIN;
			obj->m_color = float3(1.f, 0.f, 0.f);
			m_transform->m_scale = float3(100.f, 100.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 2);
		}
		else {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_SUB;
			obj->m_color = float3(1.f, 0.6f, 0.f);
			m_transform->m_scale = float3(80.f, 80.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 1);
		}
		m_transform->m_rotate.z = 0.f;
	}
}


// EOF