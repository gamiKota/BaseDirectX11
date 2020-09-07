// �z�[�~���O�~�T�C���N���X [Missile.cpp]
#include "Missile.h"
#include "GameObject.h"
#include "Camera.h"
#include "D3DClass.h"
#include "ModelManager.h"
#include "Collision.h"
#include "Score.h"
//#include "debugproc.h"
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

	// ���f�����W���擾
	XMFLOAT3 vX = { m_transform->GetMatrix()._11, m_transform->GetMatrix()._12, m_transform->GetMatrix()._13 };
	XMFLOAT3 vY = { m_transform->GetMatrix()._21, m_transform->GetMatrix()._22, m_transform->GetMatrix()._23 };
	XMFLOAT3 vZ = { m_transform->GetMatrix()._31, m_transform->GetMatrix()._32, m_transform->GetMatrix()._33 };

	// ���W�̎擾�ƃN���A
	XMFLOAT3 vP = { m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z };
	m_transform->m_position = XMFLOAT3();
	//m_transform->GetMatrix()._41 = m_transform->GetMatrix()._42 = m_transform->GetMatrix()._43 = 0.0f;
	//PrintDebugProc("Missile={%.3f,%.3f,%.3f}\n",
	//	vP.x, vP.y, vP.z);

	//// �^�[�Q�b�g���L����?
	//if (m_pEnemy) {
	//	// �G�@�ւ̃x�N�g�������߂�
	//	XMFLOAT3 vPP = m_pEnemy->GetPos();
	//	XMVECTOR vTarget = XMVectorSet(vPP.x - vP.x,
	//		vPP.y - vP.y, vPP.z - vP.z, 0.0f);
	//	// ���K��
	//	vTarget = XMVector3Normalize(vTarget);

	//	// �i�s�����Ƃ̊p�x�����߂�
	//	float fDot;
	//	XMStoreFloat(&fDot, XMVector3Dot(
	//		XMLoadFloat3(&vZ), vTarget));
	//	// �v�Z�덷�Ŕ͈͊O�ɂȂ�ꍇ�Ɋۂ߂�
	//	if (fDot < -1.0f) fDot = -1.0f;
	//	if (fDot > 1.0f) fDot = 1.0f;
	//	float fAngle = XMConvertToDegrees(acosf(fDot));
	//	if (fAngle < -3.0f) fAngle = -3.0f;
	//	if (fAngle > 3.0f) fAngle = 3.0f;

	//	// ��]�������߂�
	//	XMVECTOR vAxis = XMVector3Cross(
	//		XMLoadFloat3(&vZ), vTarget);
	//	// ���K��
	//	vAxis = XMVector3Normalize(vAxis);

	//	// ���[���h�ϊ�����]
	//	XMMATRIX rotate = XMMatrixRotationAxis(
	//		vAxis, XMConvertToRadians(fAngle));
	//	XMMATRIX world = XMLoadFloat4x4(&GetMatrix());
	//	world = XMMatrixMultiply(world, rotate);
	//	XMStoreFloat4x4(&GetMatrix(), world);
	//	//PrintDebugProc("World={%.3f,%.3f,%.3f}\n",
	//	//	GetMatrix()._41, GetMatrix()._42, GetMatrix()._43);
	//}

	// ���x�����W�ɔ��f
	if (1) {
		vP.x += vZ.x * SPEED;
		vP.y += vZ.y * SPEED;
		vP.z += vZ.z * SPEED;
	}
	// ���W�����[���h�s��ɔ��f
	m_transform->m_position.x = vP.x;
	m_transform->m_position.y = vP.y;
	m_transform->m_position.z = vP.z;

	// ���f���̍X�V
	ModelManager::GetInstance().Update(E_MODEL_MISSILE);

	std::list<GameObject*> list = GameObject::FindGameObjectsWithTag("Enemy");
	for (auto obj : list) {
		if (m_gameObject->GetComponent<Collision>() != nullptr && obj->GetComponent<Collision>() != nullptr) {
			if (Collision::AABB(*m_gameObject->GetComponent<Collision>(), *obj->GetComponent<Collision>())) {
				Destroy(obj);
				Destroy(m_gameObject);
				if (GameObject::Find("Score") != nullptr) {
					GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
				}
			}
		}
	}
}

// �`��
void Bullet::Draw()
{
	if (m_nStat == 0) {
		return;
	}

	//XMMATRIX matrix = XMLoadFloat4x4(&m_transform->GetMatrix());	// �s��(�g�k�A��]�A���W���蓮�ŕύX����ꍇ)

	//// �g�k�̕ύX
	////matrix = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), matrix);
	//// ��]���̕ύX
	//matrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)), matrix);
	//// ���W�̕ύX
	////matrix = XMMatrixMultiply(XMMatrixTranslation(0.f, 175.f, 0.f), matrix);

	//XMFLOAT4X4 world;
	//XMStoreFloat4x4(&world, matrix);

	ModelManager::GetInstance().Draw(E_MODEL_MISSILE, m_transform->GetMatrix());
}

//void Bullet::OnCollision(GameObject* obj) {
//	if (obj->GetTag() == "Enemy") {
//		GameObject::Destroy(obj);
//	}
//}

// ����
void Bullet::Fire(XMFLOAT4X4 pWorld, XMFLOAT3 pOffset) {
	XMFLOAT3 vPos;
	XMStoreFloat3(&vPos,
		XMVector3TransformCoord(
			XMLoadFloat3(&pOffset),
			XMLoadFloat4x4(&pWorld)));

	m_transform->GetMatrix() = pWorld;
	m_transform->GetMatrix()._41 = vPos.x;
	m_transform->GetMatrix()._42 = vPos.y;
	m_transform->GetMatrix()._43 = vPos.z;

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