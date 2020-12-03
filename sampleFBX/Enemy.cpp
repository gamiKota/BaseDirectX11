/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "System.h"


void Enemy::Start() {
	m_gameObject->AddComponent<Collision>();
	m_gameObject->AddComponent<Rigidbody>();
}


void Enemy::Update() {

}


// EOF