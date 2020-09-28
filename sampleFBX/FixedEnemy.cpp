/**
 * @file FixedEnemy.cpp
 */


/**
 * include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"


void FixedEnemy::Start() {
	Enemy::Start();
}


void FixedEnemy::Update() {
	GameObject* target = GameObject::Find("Player");
	if (target != nullptr) {
		m_transform->LookAt(target->m_transform);
	}
}


// EOF