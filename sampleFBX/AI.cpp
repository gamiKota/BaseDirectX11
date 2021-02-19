#include "AI.h"
#include "Frame.h"
#include "System.h"

AI::AI() : m_lock(false), m_time(0.f), m_maxTime(0.f){

}

void AI::Update() {
	if (m_isStart) {
		// 開始時のコールバック関数
		if (m_start != nullptr) {
			m_start();
			m_start = nullptr;
		}
		m_isStart = false;
	}
	if (m_time < m_maxTime) {
		m_time += Frame::GetInstance().GetDeltaTime();
		if (m_time >= m_maxTime) {
			Init();
			// 終了時のコールバック関数
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

	m_isStart = true;

	return this;
}

void AI::Init() {
	m_time = m_maxTime = 0.f;
	m_lock = false;
}


// EOF