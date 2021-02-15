/**
 * @Character.cpp
 */


/**
 * @include
 */
#include "Character.h"
#include "Status.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "GameObject.h"

#include "System.h"


void Character::Init() {
	// キャラクターが標準で持つコンポーネントの追加
	m_status = m_gameObject->AddComponent<Status>();
	m_collision = m_gameObject->AddComponent<CollisionBox>();
	m_collider = m_gameObject->AddComponent<Collider>();
}


// EOF