// �G�@�N���X [Enemy.cpp]
#include "Enemy.h"
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

// ������
void CEnemy::Start() {
	
}

// �I������
void CEnemy::Uninit() {

}

// �X�V
void CEnemy::Update() {
	//if (m_transform->m_position.z < GameObject::Find("Player")->m_transform->m_position.z) {
	//	Destroy(m_gameObject);
	//}
}


// EOF