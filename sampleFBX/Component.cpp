/**
 * @file Component.cpp
 */


/**
 * @include
 */
#include "Component.h"
#include "GameObject.h"
#include "System.h"


Component::Component() : Object("Component") {
	m_transform = &m_gameObject->m_transform;
}

Component::~Component() {
	
}

void Component::Awake() {

}

void Component::Start() {

}

void Component::Uninit() {

}

void Component::Update() {

}

void Component::Draw() {

}



// EOF