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

}


// EOF