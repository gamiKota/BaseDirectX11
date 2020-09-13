/**
 * @file Enemy.cpp
 */


/**
 * @include
 */
#include "Enemy.h"
#include "GameObject.h"
#include "Collision.h"
#include "System.h"


void Enemy::Start() {
	m_gameObject->AddComponent<Collision>();
	m_gameObject->GetComponent<Collision>()->Init(E_MODEL_ENEMY);
}


void Enemy::Update() {

}


// EOF