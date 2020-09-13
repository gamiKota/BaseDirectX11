/**
 * @file Sky.cpp
 */


/**
 * @include
 */
#include "Sky.h"
#include "GameObject.h"
#include "System.h"


void SkyDome::Start() {

}


void SkyDome::Update() {
	GameObject* player = GameObject::Find("Player");
	if (player != nullptr) {
		m_transform->m_position = player->m_transform->m_position;
	}
}

// EOF