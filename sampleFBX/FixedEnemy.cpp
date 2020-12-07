/**
 * @file FixedEnemy.cpp
 */


/**
 * @include
 */
#include "FixedEnemy.h"
#include "GameObject3D.h"
#include "Player.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "TargetCtr.h"
#include "debugproc.h"
#include "System.h"


void FixedEnemy::Start() {
	// “G‚Ì‹¤’Êˆ—
	Enemy::Start();

	// •Ï”‚Ì‰Šú‰»
	m_status->m_HP = 500.f;
	m_status->m_AttakPower = 10.f;
}


void FixedEnemy::Update() {
	Enemy::Update();
	GameObject* target = GameObject::Find("Player");
	if (target != nullptr) {
		m_transform->LookAt(target->m_transform);
	}
}


void FixedEnemy::OnCollision(GameObject* obj) {

}


// EOF