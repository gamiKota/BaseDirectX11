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


float3 LockOnMarker(float3 target) {
	float3 OutPos = float3();

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
	XMVECTOR vViewProj = XMLoadFloat3(&target);

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

	if (m_target == nullptr) {
		m_target = GameObject::FindGameObjectWithTag("Enemy");
	}
}


void TargetCtr::Update() {
	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	if (m_target == GameObject::Find("Player")->GetComponent<PlayerCtr>()->m_target) {
		obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_MAIN;
		obj->m_color = float3(1.f, 0.f, 0.f);
	}
	else {
		obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_SUB;
		obj->m_color = float3(1.f, 0.6f, 0.f);
	}
	
	// ���b�N�I���}�[�J�[
	m_transform->m_position = LockOnMarker(m_target->m_transform->m_position);
}


// EOF