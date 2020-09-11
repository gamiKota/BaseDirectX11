// �z�[�~���O�~�T�C���N���X [Missile.cpp]
#include "Missile.h"
#include "GameObject.h"
#include "Collision.h"
#include "Score.h"
#include "Player.h"
#include "System.h"

// �}�N����`
#define SPEED			50.0f		// ����


// �R���X�g���N�^
Bullet::Bullet() : m_nStat(0), m_nLife(0) {
}

// �f�X�g���N�^
Bullet::~Bullet() {

}


void Bullet::Awake() {

}

// ������
void Bullet::Start() {
	m_nLife = 0;
	m_nStat = 0;
}

// �I������
void Bullet::Uninit() {
}

// �X�V
void Bullet::Update() {
	// ���g�p?
	if (m_nStat == 0) {
		return;
	}
	// �ǔ��� (m_nStat == 1)
	--m_nLife;
	if (m_nLife <= 0) {
		// �����J�n
		m_nLife = 3 * 60;
		m_nStat = 0;
		GameObject::Destroy(m_gameObject);
		return;
	}
	// �O�ɐi��
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	if (obj->GetTag() == "Enemy") {
		GameObject::Destroy(obj);
		Destroy(m_gameObject);
		if (GameObject::Find("Score") != nullptr) {
			GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
		}
		if (GameObject::Find("Player") != nullptr) {
			if (obj == GameObject::Find("Player")->GetComponent<CPlayer>()->m_target)
				GameObject::Find("Player")->GetComponent<CPlayer>()->m_target = nullptr;
		}
	}
}

// ����
void Bullet::Fire(Transform transform) {

	m_transform->m_position = transform.m_position;
	m_transform->m_rotate = transform.m_rotate;
	m_transform->m_scale = { 1.f, 1.f, 1.f };


	m_nLife = 1 * 60;	// 5�b
	m_nStat = 1;		// �ǔ���


	// �^�[�Q�b�g���L����?
	//if (m_pEnemy) {
		//// ���f�����W���擾
		//XMFLOAT3 vX = { m_transform->_11, m_transform->_12, m_transform->_13 };
		//XMFLOAT3 vY = { m_transform->_21, m_transform->_22, m_transform->_23 };
		//XMFLOAT3 vZ = { m_transform->_31, m_transform->_32, m_transform->_33 };
		//// �G�@�ւ̃x�N�g�������߂�
		//XMFLOAT3 vP = { m_transform->_41, m_transform->_42, m_transform->_43 };
		//XMFLOAT3 vPP = XMFLOAT3();// = m_pEnemy->GetPos();
		//XMVECTOR vTarget = XMVectorSet(vPP.x - vP.x,
		//	vPP.y - vP.y, vPP.z - vP.z, 0.0f);
		////���K��
		//vTarget = XMVector3Normalize(vTarget);

		//// �i�s�����Ƃ̊p�x�����߂�
		//float fDot;
		//XMStoreFloat(&fDot, XMVector3Dot(
		//	XMLoadFloat3(&vZ), vTarget));
		//// �v�Z�덷�Ŕ͈͊O�ɂȂ�ꍇ�Ɋۂ߂�
		//if (fDot < -1.0f) fDot = -1.0f;
		//if (fDot > 1.0f) fDot = 1.0f;
		//float fAngle = XMConvertToDegrees(acosf(fDot));
		//if (fAngle < -3.0f) fAngle = -3.0f;
		//if (fAngle > 3.0f) fAngle = 3.0f;

		//// ��]�������߂�
		//XMVECTOR vAxis = XMVector3Cross(
		//	XMLoadFloat3(&vZ), vTarget);
		//// ���K��
		//vAxis = XMVector3Normalize(vAxis);

		// ���[���h�ϊ�����]
		//XMMATRIX rotate = XMMatrixRotationAxis(
		//	vAxis, XMConvertToRadians(fAngle));
		//XMMATRIX world = XMLoadFloat4x4(m_transform);
		//world = XMMatrixMultiply(world, rotate);
		//XMStoreFloat4x4(m_transform, world);
		//PrintDebugProc("World={%.3f,%.3f,%.3f}\n",
		//	m_transform->_41, m_transform->_42, m_transform->_43);
	//}
}

// EOF