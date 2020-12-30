/**
 * @PlayerState.cpp
 */


/**
 * @include
 */
// ���g�̐錾��
#include "PlayerState.h"
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
void PlayerState::Initialize() {
	// ��Ԃ̒ǉ�
	StateMachine::AddState(new PlayerState::Idol(this), true);
	StateMachine::AddState(new PlayerState::Move(this));
	StateMachine::AddState(new PlayerState::TargetOn(this));
	StateMachine::AddState(new PlayerState::TargetOff(this), true);
	StateMachine::AddState(new PlayerState::AttackBullet(this));

	// �ϐ��̏�����
	m_roll = 0.f;
}

// ��ԂɈˑ����Ȃ����ʏ���
void PlayerState::Update() {
	StateMachine::Update();
	m_transform->m_rotate.z = XMConvertToRadians(m_roll);
}


/**************************************************************************************************
 * @state �Î~
 *************************************************************************************************/
void PlayerState::Idol::Start() {
	main->SetStateActive(PLAYER_STATE::MOVE, false);
}

void PlayerState::Idol::Update() {
	if (main->m_roll > 0.f) {
		main->m_roll -= VAL_ANGLE_Z * 0.5f;
	}
	else if (main->m_roll < 0.f) {
		main->m_roll += VAL_ANGLE_Z * 0.5f;
	}
}

void PlayerState::Idol::OnDestoy() {
}


/**************************************************************************************************
 * @state �ړ�
 *************************************************************************************************/
void PlayerState::Move::Start() {
	main->SetStateActive(PLAYER_STATE::IDOL, false);
}

void PlayerState::Move::Update() {
	// ���f���p���Ɉˑ����Ȃ����s�ړ�
	XMFLOAT4X4 mtx = XMFLOAT4X4();
	XMStoreFloat4x4(&mtx, XMMatrixRotationRollPitchYaw(main->m_transform->m_rotate.x, main->m_transform->m_rotate.y, 0.f));
	float3 right = float3(mtx._11, mtx._12, mtx._13);
	float3 up = float3(mtx._21, mtx._22, mtx._23);
	float3 forward = float3(mtx._31, mtx._32, mtx._33);

	// Z���ړ�
	if (main->m_movement.z > 0.f) {
		main->m_transform->m_position += forward * SPEED;
	}
	else if (main->m_movement.z < 0.f) {
		main->m_transform->m_position -= forward * SPEED;
	}
	// X���ړ�
	if (main->m_movement.x > 0.f) {
		main->m_transform->m_position += right * SPEED;
		if (main->m_roll >= -MAX_ANGLE_Z)
			main->m_roll -= VAL_ANGLE_Z;
	}
	else if (main->m_movement.x < 0.f) {
		main->m_transform->m_position -= right * SPEED;
		if (main->m_roll <= MAX_ANGLE_Z)
			main->m_roll += VAL_ANGLE_Z;
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
	if (main->m_movement.y > 0.f) {
		main->m_transform->m_position.y += SPEED;
	}
	else if (main->m_movement.y < 0.f) {
		main->m_transform->m_position.y -= SPEED;
	}
}

void PlayerState::Move::OnDestoy() {
}


/**************************************************************************************************
 * @state �^�[�Q�b�gON
 *************************************************************************************************/
void PlayerState::TargetOn::Start() {
	main->m_target = nullptr;
	main->SetStateActive(PLAYER_STATE::TARGET_OFF, false);

	// ����
	// �����G��������߂���(�������̓\�[�g��Ȃ�)��
	// ���ԂɓG�����b�N�I������
	// ���̂܂܂��ƃ��X�g��1�Ԗڂ�2�Ԗڂ����݂Ɍ������邾��
	std::list<GameObject*> objlist = GameObject::FindGameObjectsWithTag("Enemy");
	for (auto obj : objlist) {
		if (main->m_target != obj) {		// ���łɃ��b�N�I����ԂȂ̂ŁA�Ⴄ�G�����b�N�I��
			main->m_target = obj;
			break;
		}
	}
}

void PlayerState::TargetOn::Update() {
	if (main->m_target != nullptr) {
		bool b = false;
		std::list<GameObject*> objlist = GameObject::FindGameObjectsWithTag("Enemy");
		for (auto obj : objlist) {
			if (main->m_target == obj) {		// �^�[�Q�b�g�ƈ�v
				b = true;
				main->m_transform->LookAt(main->m_target->m_transform);
				break;
			}
		}
		if (!b) {	// �^�[�Q�b�g�����ł��Ă�ꍇ
			main->SetStateActive(PLAYER_STATE::TARGET_OFF, true);
		}
	}
}

void PlayerState::TargetOn::OnDestoy() {
	main->m_target = nullptr;
}


/**************************************************************************************************
 * @state �^�[�Q�b�gOFF
 *************************************************************************************************/
void PlayerState::TargetOff::Start() {
	main->SetStateActive(PLAYER_STATE::TARGET_ON, false);
}

void PlayerState::TargetOff::Update() {
	main->m_transform->m_rotate.y = 0.f;	// Y���ɉ�]���ė~�����Ȃ�
}

void PlayerState::TargetOff::OnDestoy() {
}


/**************************************************************************************************
 * @state �ˌ�
 *************************************************************************************************/
void PlayerState::AttackBullet::Start() {
	GameObject* obj = new GameObject3D(E_MODEL_BULLET, "Bullet", "BulletPlayer");
	Instantiate(obj, main->m_transform->m_position + main->m_transform->m_forward * 200.f, main->m_transform->m_rotate);
	obj->AddComponent<Bullet>();
	// Start�֐��Ō����I������̂ŏ�ԏI��
	main->SetStateActive(PLAYER_STATE::ATTACK_BULLET, false);
}

void PlayerState::AttackBullet::Update() {
}

void PlayerState::AttackBullet::OnDestoy() {
}



// EOF