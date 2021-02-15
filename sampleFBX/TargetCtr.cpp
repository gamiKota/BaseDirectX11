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
#include "PlayerState.h"
#include "Collision.h"
#include "Material.h"
#include "System.h"


using namespace DirectX;


static const float CIRCLE_SIZE = 300.f;



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
	// z�𐳋K�����Ȃ��ƕ��̒l�����̒l�ŃJ�����̌�딻�肪�Ƃꂻ��
	XMFLOAT3 norPos;
	XMStoreFloat3(&norPos, vViewProj);
	norPos.x /= norPos.z; norPos.y /= norPos.z;
	//norPos.z /= norPos.z;

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
	obj->m_ps = E_PS::PS_COLOR;

	if (m_target == nullptr) {
		m_target = GameObject::FindGameObjectWithTag("Enemy");
	}
}


void TargetCtr::Update() {
	// ���b�N�I���}�[�J�[
	float3 centerPos = m_target->GetComponent<CollisionBox>()->m_vCenter;
	float3 marker = LockOnMarker(m_target->m_transform->m_position);
	m_transform->m_position = marker;

	GameObjectUI* obj = dynamic_cast<GameObjectUI*>(m_gameObject);
	obj->m_material->m_diffuse.w = 0.f;

	// ����
	// ���C���^�[�Q�b�g�ȊO�̃��b�N�I���}�[�J�[�̕\���́A
	// �T�u�^�[�Q�b�g�̓��C���^�[�Q�b�g��؂�ւ���1�b�A�������͉���������U�����Ă���G
	// ��ʊO�~�j���b�N�I���}�[�J�[�͉���������U�����Ă��鎞�A��e���ɂ�ATTACK�ɐ؂�ւ���1�b�\��
	if (marker.z < 0.f ||
		(m_transform->m_position.x + m_transform->m_scale.x * 0.5f < -(float)SCREEN_CENTER_X ||
		 m_transform->m_position.x - m_transform->m_scale.x * 0.5f >  (float)SCREEN_CENTER_X ||
		 m_transform->m_position.y + m_transform->m_scale.y * 0.5f < -(float)SCREEN_CENTER_Y ||
		 m_transform->m_position.y - m_transform->m_scale.y * 0.5f >  (float)SCREEN_CENTER_Y)) {	// ��ʊO
		// �v���C���[�Ƒ���̍��W����x�N�g�������߂�
		// ���̈ʒu�֌W���X�N���[���Ɏ����Ă���
		// �^�ォ�猩��(�~�j�}�b�v�H)
		// �^�ォ�猩�Ĉʒu�֌W(�p�x)������o��(����͉~�\���ɂȂ�)
		// ��ʔ�Ŋ�������Ȃ񂩓��������ۂ�
		// ��ʓ��Ɏ��܂�悤�ɐ��K��
		// ��]���͎�荇�����v���C���[��Y����]�������Ă���(�����ƈ��S�ȏ�񂪗ǂ�)
		// ���b�N�I����Ԃ��ƒ����_���v���C���[���W����^�[�Q�b�g���W�ɐ؂�ւ�邽�߁A
		// ���o�I�ɂ̓Y�����l�ɕ\������Ă��܂�

		// ���b�N�I������3D���W��Ńx�N�g�����o���Ă�������X�N���[���ϊ�����ԊȒP����

		//GameObject* player = GameObject::Find("Player");
		//CCamera* camera = CCamera::Get();

		//// ���W�̏�����
		//m_transform->m_position = float3();
		//float tempX;
		//float tempZ;
		//float vecX;
		//float vecZ;
		//GameObject* playerTarget = player->GetComponent<PlayerState>()->GetTarget();
		////float3 playerMaker = LockOnMarker(player->m_transform);

		//// �v���C���[�̃^�[�Q�b�g�w���Ԃŏ����𕪂���
		//// �^�[�Q�b�g�Ɩ������Ă鎞�ɋ������΂���
		//if (playerTarget != nullptr) {
		//	// 3D��ł̃x�N�g��
		//	float3 vec3D = m_target->m_transform->m_position - playerTarget->m_transform->m_position;
		//	// ���K��
		//	vec3D = float3::Normalize(vec3D);
		//	// ���W�Еϊ�
		//	float3 pos3D = playerTarget->m_transform->m_position;
		//	pos3D += vec3D;
		//	// �X�N���[�����W�ɕϊ�
		//	pos3D = LockOnMarker(pos3D);
		//	// �␳(�������Ȃ����������킩��Ȃ�)
		//	if (pos3D.y > 1.f) {
		//		pos3D.y = m_colY;
		//	}
		//	else {
		//		m_colY = pos3D.y;
		//	}
		//	// ��ʔ�ɍ��킹���x�N�g���ɕϊ�
		//	pos3D *= (float)SCREEN_RATIO;
		//	// ���K��
		//	pos3D = float3::Normalize(float3(pos3D.x, pos3D.y, 0.f)) * CIRCLE_SIZE;
		//	// ���W�ɔ��f
		//	m_transform->m_position += pos3D;
		//	// �^�[�Q�b�g�����̍X�V
		//	float angle = atan2(m_transform->m_position.y, m_transform->m_position.x);
		//	m_transform->m_rotation = Quaternion::AngleAxis(angle, float3(0.f, 0.f, 1.f));
		//}
		//else {
		//	// �񎟌��x�N�g���ɕϊ�
		//	float3 playerRot = Quaternion::RadianAngle(player->m_transform->m_rotation);
		//	tempX = m_target->m_transform->m_position.x - player->m_transform->m_position.x;
		//	tempZ = m_target->m_transform->m_position.z - player->m_transform->m_position.z;
		//	vecX = tempX * cosf(playerRot.y) - tempZ * sinf(playerRot.y);
		//	vecZ = tempX * sinf(playerRot.y) + tempZ * cosf(playerRot.y);
		//	// ��ʔ�ɍ��킹���x�N�g���ɕ␳
		//	vecX *= (float)SCREEN_RATIO;
		//	vecZ *= (float)SCREEN_RATIO;
		//	// ���K��
		//	float3 vec = float3::Normalize(float3(vecX, vecZ, 0.f)) * CIRCLE_SIZE;
		//	// ���W�ɔ��f
		//	m_transform->m_position += vec;
		//	// �^�[�Q�b�g�����̍X�V
		//	float angle = XMConvertToDegrees(atan2(m_transform->m_position.y, m_transform->m_position.x));
		//	m_transform->m_rotation = Quaternion::AngleAxis(angle, float3(0.f, 0.f, 1.f));
		//}

		//obj->m_texture = E_TEXTURE_ROCK_ICON_OUTCAMERA_MINI;
		//obj->m_color = float3(1.f, 0.f, 0.f);
		//obj->m_layer = E_LAYER::UI;

	}
	else {	// ��ʓ�
		if (m_target == GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget()) {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_MAIN;
			m_transform->m_scale = float3(100.f, 100.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 2);
			obj->m_material->m_diffuse = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		}
		else {
			obj->m_texture = E_TEXTURE_ROCK_ICON_INCAMERA_SUB;
			obj->m_material->m_diffuse = XMFLOAT4(1.f, 0.6f, 0.f, 1.f);
			m_transform->m_scale = float3(80.f, 80.f, 0.f);
			obj->m_layer = (E_LAYER)((int)E_LAYER::UI + 1);
		}
		m_transform->m_rotation = Quaternion::AngleAxis(0.f, float3(0.f, 0.f, 1.f));
	}
}


// EOF