/**
 * @file Sky.cpp
 */


/**
 * @include
 */
#include "Sky.h"
#include "GameObject.h"
#include "Camera.h"
#include "System.h"


void SkyDome::Start() {

}


void SkyDome::Update() {
	m_transform->m_position = CCamera::Get()->m_transform->m_position;
}

// EOF