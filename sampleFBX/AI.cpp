#include "AI.h"
#include "Frame.h"
#include "System.h"

AI::AI() : m_lock(false), m_time(0.f), m_maxTime(0.f){

}

void AI::Update() {
	if (m_time < m_maxTime) {
		m_time += Frame::GetInstance().GetDeltaTime();
		if (m_time >= m_maxTime) {
			Init();
			if (m_comp != nullptr) {
				m_comp();
				m_comp = nullptr;
			}
		}
	}
}

AI* AI::StartUp(float waitTime, bool lock) {
	if (m_lock) { return this; }

	m_maxTime = waitTime;
	m_time = 0.f;
	m_lock = lock;

	return this;
}

void AI::Init() {
	m_time = m_maxTime = 0.f;
	m_lock = false;
}


// EOF