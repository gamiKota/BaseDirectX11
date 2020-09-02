// �G�@�N���X [Enemy.cpp]
#include "Enemy.h"
#include "input.h"
#include "D3DClass.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "System.h"

// �}�N����`
#define SPEED			15.0f	// ����


// �R���X�g���N�^
CEnemy::CEnemy() : Component() {
}

// �f�X�g���N�^
CEnemy::~CEnemy() {
}


void CEnemy::Awake() {
	m_transform = &m_gameObject->m_transform;
}


// ������
void CEnemy::Start() {
	
}

// �I������
void CEnemy::Uninit() {

}

// �X�V
void CEnemy::Update() {

	ModelManager::GetInstance().Update(E_MODEL_ENEMY);

	if (m_transform->_43 < GameObject::Find("Player")->m_transform._43) {
		Destroy(m_gameObject);
	}
}

// �`��
void CEnemy::Draw() {

	ModelManager::GetInstance().Draw(E_MODEL_ENEMY, m_gameObject->m_transform);
}
