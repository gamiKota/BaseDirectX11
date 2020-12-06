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


void Character::Init() {
	// キャラクターが標準で持つコンポーネントの追加
	m_status = m_gameObject->AddComponent<Status>();
	m_collision = m_gameObject->AddComponent<Collision>();
	m_rigidbody = m_gameObject->AddComponent<Rigidbody>();
}


// EOF