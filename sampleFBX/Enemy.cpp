/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "GameObjectUI.h"
#include "Status.h"
#include "PlayerState.h"
#include "EnemyState.h"
#include "EnemyOperation.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "TargetCtr.h"
#include "GameObjectMesh.h"
#include "HPGauge.h"
#include "Frame.h"
#include "AI.h"
#include "System.h"


void Enemy::Start() {
	// Character�N���X�̏�����
	Character::Init();

	// �GAI�ɕK�v�ȃR���|�[�l���g�̒ǉ�
	// ����͊e�G�p���R���|�[�l���g�ōs��
	m_state = m_gameObject->AddComponent<EnemyState>();
	m_ai = m_gameObject->AddComponent<AI>();
	//m_operate = m_gameObject->AddComponent<EnemyOperation>();

	// �ϐ��̏�����(�G�N���X�̃f�t�H���g�l�̐ݒ�)
	m_collider->m_weight = E_WEIGHT::_2;	// ��ԖڂɌy��

	// ���b�N�I���}�[�J�[�̒ǉ�(�Ƃ肠�������͕\�����Ȃ�)
	GameObjectUI* obj = new GameObjectUI(E_LAYER::UI, E_TEXTURE::E_TEXTURE_ROCK_ICON_INCAMERA_MAIN, "EnemyIcon");
	obj->AddComponent<TargetCtr>()->m_target = m_gameObject;
	m_LockIcon = GameObject::Instantiate(obj);

	// �v���^�C��
	m_defaultTime = 0.f;
}


void Enemy::Uninit() {
	Destroy(m_LockIcon);
}


void Enemy::Update() {
	m_defaultTime += Frame::GetInstance().GetDeltaTime();
}


void Enemy::EnemyDelete(GameObject* obj) {
	if (GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget() != nullptr &&
		obj->GetInstanceID() ==
		GameObject::Find("Player")->GetComponent<PlayerState>()->GetTarget()->GetInstanceID()) {	// ���������^�[�Q�b�g�̏ꍇ
		GameObject::Find("Player")->GetComponent<PlayerState>()->SetTarget();
	}
	GameObject::Destroy(obj);
}


// EOF