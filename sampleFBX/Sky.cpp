/**
 * @file Sky.cpp
 */


/**
 * @include
 */
#include "Sky.h"
#include "GameObject3D.h"
#include "Camera.h"
#include "System.h"


void SkyDome::Start() {
	GameObject3D* obj = dynamic_cast<GameObject3D*>(m_gameObject);
}


void SkyDome::Update() {
	m_transform->m_position = CCamera::Get()->m_transform->m_position;
}

// EOF