/**
 * @file Rigidbody.cpp
 * @brief 物理エンジン機能
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


Collider::Collider() : Component(), m_weight(E_WEIGHT::_0) {

}


void Collider::Uninit() {

}


void Collider::Update() {

}


void Collider::LastUpdate() {

}


void Collider::ShiftCollision(GameObject* obj1, GameObject* obj2) {


	if (obj1->GetComponent<Collider>()->m_weight > E_WEIGHT::_5 &&
		obj1->GetComponent<Collider>()->m_weight > E_WEIGHT::_5) {
		return;
	}

	// 添え字0が軽い方
	GameObject* obj[2] = {
		obj1, obj2
	};

	// 重さが軽い方が動く
	if (obj1->GetComponent<Collider>()->m_weight > obj2->GetComponent<Collider>()->m_weight) {
		obj[0] = obj2;
		obj[1] = obj1;
	}
	// 今の所同じ場合は考えない
	//else if (obj1->GetComponent<Rigidbody>()->m_weight == obj2->GetComponent<Rigidbody>()->m_weight) {
	//
	//}



	BYTE i = 0;
	while (true) {	// めり込んだ分だけ押し戻す

		//if (i >= 99999999) {
		//	MessageBox(NULL, L"Rigidbody", NULL, MB_OK);
		//}

		if (obj[1]->GetComponent<Collider>()->m_weight == E_WEIGHT::_WALL) {
			// 少しずつずらす
			float3 v = obj[1]->m_transform->m_forward;
			obj[0]->m_transform->m_position += v;
		}
		else if (obj[1]->GetComponent<Collider>()->m_weight == E_WEIGHT::_LAND) {
			// 少しずつずらす
			float3 v = obj[1]->m_transform->m_up;
			obj[0]->m_transform->m_position += v;
		}
		else {
			// 自分と相手のベクトル
			float3 vec = obj[1]->m_transform->m_position - obj[0]->m_transform->m_position;
			vec = float3::Normalize(vec);

			// 少しずつずらす
			obj[0]->m_transform->m_position -= vec * 1.f;
		}

		// 必要な情報の更新処理
		obj[0]->GetComponent<CollisionBox>()->LastUpdate();
		obj[0]->GetComponent<Transform>()->LastUpdate();
		obj[1]->GetComponent<CollisionBox>()->LastUpdate();
		obj[1]->GetComponent<Transform>()->LastUpdate();

		// 当たらなくなったらループから出る
		if (!CollisionBox::OBB(*obj1->GetComponent<CollisionBox>(), *obj2->GetComponent<CollisionBox>())) {
			break;
		}

		i += 1;
	}
}


void Collider::SetImGuiVal() {
#if _DEBUG
#endif
}


// EOF