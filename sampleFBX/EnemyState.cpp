/**
 * @EnemyState.cpp
 */


/**
 * @include
 */
 // ���g�̐錾��
#include "EnemyState.h"
// �I�u�W�F�N�g�V�X�e��
#include "Transform.h"
#include "GameObject.h"
#include "GameObject3D.h"
// �R���|�[�l���g
#include "Bullet.h"
// �V�X�e��
#include "debugproc.h"
#include "System.h"


/**
 * @abridgement namespace
 */
using namespace DirectX;


/**
 * @constant
 */
static const float SPEED = 15.0f;	// ����
static const float VAL_ANGLE_Z = 2.f;
static const float MAX_ANGLE_Z = 30.f;


// �X�e�[�g�}�V���̏�����
void EnemyState::Initialize() {
	// ��Ԃ̒ǉ�
	StateMachine::AddState(new EnemyState::Idol(this), true);
	StateMachine::AddState(new EnemyState::Move(this));
	StateMachine::AddState(new EnemyState::TargetOn(this));
	StateMachine::AddState(new EnemyState::TargetOff(this));
	StateMachine::AddState(new EnemyState::AttackBullet(this));

	// �ϐ��̏�����
	m_roll = 0.f;
}

// ��ԂɈˑ����Ȃ����ʏ���
void EnemyState::Update() {
	StateMachine::Update();
	m_transform->m_rotate.z = XMConvertToRadians(m_roll);
}


/**************************************************************************************************
 * @state �Î~
 *************************************************************************************************/
void EnemyState::Idol::Start() {
	main->SetStateActive(ENEMY_STATE::MOVE, false);
}

void EnemyState::Idol::Update() {
	if (main->m_roll > 0.f) {
		main->m_roll -= VAL_ANGLE_Z * 0.5f;
	}
	else if (main->m_roll < 0.f) {
		main->m_roll += VAL_ANGLE_Z * 0.5f;
	}
	PrintDebugProc("Idol\n");
}

void EnemyState::Idol::OnDestoy() {
}


/**************************************************************************************************
 * @state �ړ�
 *************************************************************************************************/
void EnemyState::Move::Start() {
	main->SetStateActive(ENEMY_STATE::IDOL, false);
}

void EnemyState::Move::Update() {
	// ���f���p���Ɉˑ����Ȃ����s�ړ�
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(main->m_transform->m_rotate.x, main->m_transform->m_rotate.y, 0.f));
	float3 right = float3(mtx._11, mtx._12, mtx._13);
	float3 up = float3(mtx._21, mtx._22, mtx._23);
	float3 forward = float3(mtx._31, mtx._32, mtx._33);

	// Z���ړ�
	if (main->m_movement.z != 0.f) {
		main->m_transform->m_position += forward * (SPEED * main->m_movement.z);
	}
	// X���ړ�
	if (main->m_movement.x != 0.f) {
		main->m_transform->m_position += right * (SPEED * main->m_movement.x);
		if (main->m_movement.x > 0.f) {
			if (main->m_roll >= -MAX_ANGLE_Z)
				main->m_roll -= VAL_ANGLE_Z;
		}
		else {
			if (main->m_roll <= MAX_ANGLE_Z)
				main->m_roll += VAL_ANGLE_Z;
		}
	}
	else {
		if (main->m_roll > 0.f) {
			main->m_roll -= VAL_ANGLE_Z * 0.5f;
		}
		else if (main->m_roll < 0.f) {
			main->m_roll += VAL_ANGLE_Z * 0.5f;
		}
	}
	// Y���ړ�(�^�[�Q�b�g�̕��Ɍ����Ă�̂�y�v�f�Œ��ڈړ�����)
	if (main->m_movement.y != 0.f) {
		main->m_transform->m_position.y += (SPEED * main->m_movement.y);
	}

	PrintDebugProc("Move\n");
}

void EnemyState::Move::OnDestoy() {
}


/**************************************************************************************************
 * @state �^�[�Q�b�gON
 *************************************************************************************************/
void EnemyState::TargetOn::Start() {
	// �ϐ��̏�����
	main->m_target = nullptr;
	main->SetStateActive(ENEMY_STATE::TARGET_OFF, false);
}

void EnemyState::TargetOn::Update() {
	if (main->m_target == nullptr) {
		main->SetStateActive(ENEMY_STATE::TARGET_OFF, true);
		return;
	}
	main->m_transform->LookAt(main->m_target->m_transform);
	PrintDebugProc("TargetOn\n");
}

void EnemyState::TargetOn::OnDestoy() {
	main->m_target = nullptr;
}


/**************************************************************************************************
 * @state �^�[�Q�b�gOFF(�G�������^�[�Q�b�g���ĂȂ�������H)
 *************************************************************************************************/
void EnemyState::TargetOff::Start() {
	main->SetStateActive(ENEMY_STATE::TARGET_ON, false);
}

void EnemyState::TargetOff::Update() {
	main->m_transform->m_rotate.y = 0.f;	// Y���ɉ�]���ė~�����Ȃ�
	PrintDebugProc("TargetOff\n");
}

void EnemyState::TargetOff::OnDestoy() {
}


/**************************************************************************************************
 * @state �ˌ�
 *************************************************************************************************/
void EnemyState::AttackBullet::Start() {
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, main->m_transform->m_position + main->m_transform->m_forward * 200.f, main->m_transform->m_rotate);
	obj->AddComponent<Bullet>();
	// Start�֐��Ō����I������̂ŏ�ԏI��
	main->SetStateActive(ENEMY_STATE::ATTACK_BULLET, false);
}

void EnemyState::AttackBullet::Update() {
}

void EnemyState::AttackBullet::OnDestoy() {
}


// EOF