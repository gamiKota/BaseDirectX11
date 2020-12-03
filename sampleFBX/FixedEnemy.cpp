/**
 * @file FixedEnemy.cpp
 */


/**
 * include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"
#include "Player.h"
#include "Collision.h"
#include "Transform.h"
#include "debugproc.h"
#include "System.h"


void FixedEnemy::Start() {
	Enemy::Start();
}


void FixedEnemy::Update() {
	GameObject* target = GameObject::Find("Player");
	if (target != nullptr) {
		m_transform->LookAt(target->m_transform);
	}
}


void FixedEnemy::OnCollision(GameObject* obj) {
	if (obj->ToString() == "Player") {	// プレイヤーとの当たり判定

		while (true) {	// めり込んだ分だけ押し戻す
			// 自分と相手のベクトル
			float3 vec = obj->m_transform->m_position - m_transform->m_position;
			float scalar = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			vec /= scalar;

			// 少しずつずらす
			m_transform->m_position -= vec * 1.f;

			// 必要な情報の更新処理
			m_gameObject->GetComponent<Collision>()->LastUpdate();
			m_gameObject->GetComponent<Transform>()->LastUpdate();
			obj->GetComponent<Collision>()->LastUpdate();
			obj->GetComponent<Transform>()->LastUpdate();

			// 当たらなくなったらループから出る
			if (! Collision::OBB(*m_gameObject->GetComponent<Collision>(), *obj->GetComponent<Collision>())) {
				break;
			}
		}

	}
}


// EOF