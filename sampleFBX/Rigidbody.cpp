/**
 * @file Rigidbody.cpp
 * @brief �����G���W���@�\
 */


/**
 * @include
 */
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collision.h"
#include "System.h"


static const float VAL_GRAVITY = 0.01f;


Rigidbody::Rigidbody() : Component(), m_weight(E_WEIGHT::_0) {

}


void Rigidbody::Uninit() {

}


void Rigidbody::Update() {

}


void Rigidbody::LastUpdate() {

}


void Rigidbody::ShiftCollision(GameObject* obj1, GameObject* obj2) {


	if (obj1->GetComponent<Rigidbody>()->m_weight > E_WEIGHT::_5 &&
		obj1->GetComponent<Rigidbody>()->m_weight > E_WEIGHT::_5) {
		return;
	}

	// �Y����0���y����
	GameObject* obj[2] = {
		obj1, obj2
	};

	// �d�����y����������
	if (obj1->GetComponent<Rigidbody>()->m_weight > obj2->GetComponent<Rigidbody>()->m_weight) {
		obj[0] = obj2;
		obj[1] = obj1;
	}
	// ���̏������ꍇ�͍l���Ȃ�
	//else if (obj1->GetComponent<Rigidbody>()->m_weight == obj2->GetComponent<Rigidbody>()->m_weight) {
	//
	//}



	BYTE i = 0;
	while (true) {	// �߂荞�񂾕����������߂�

		//if (i >= 99999999) {
		//	MessageBox(NULL, L"Rigidbody", NULL, MB_OK);
		//}

		if (obj[1]->GetComponent<Rigidbody>()->m_weight == E_WEIGHT::_WALL) {
			// ���������炷
			float3 v = obj[1]->m_transform->m_forward;
			obj[0]->m_transform->m_position += v;
		}
		else if (obj[1]->GetComponent<Rigidbody>()->m_weight == E_WEIGHT::_LAND) {
			// ���������炷
			float3 v = obj[1]->m_transform->m_up;
			obj[0]->m_transform->m_position += v;
		}
		else {
			// �����Ƒ���̃x�N�g��
			float3 vec = obj[1]->m_transform->m_position - obj[0]->m_transform->m_position;
			float scalar = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			vec /= scalar;

			// ���������炷
			obj[0]->m_transform->m_position -= vec * 1.f;
		}

		// �K�v�ȏ��̍X�V����
		obj[0]->GetComponent<Collision>()->LastUpdate();
		obj[0]->GetComponent<Transform>()->LastUpdate();
		obj[1]->GetComponent<Collision>()->LastUpdate();
		obj[1]->GetComponent<Transform>()->LastUpdate();

		// ������Ȃ��Ȃ����烋�[�v����o��
		if (!Collision::OBB(*obj1->GetComponent<Collision>(), *obj2->GetComponent<Collision>())) {
			break;
		}

		i += 1;
	}
}


void Rigidbody::SetImGuiVal() {

}


// EOF