#include "HPGauge.h"
#include "Transform.h"
#include "GameObject.h"
#include "System.h"

void HPGauge::Start() {

}

void HPGauge::Update() {
	m_transform->m_position = m_pair->m_transform->m_position;
	m_transform->m_position.y += 100.f;
}

void HPGauge::LastUpdate() {
}

// EOF