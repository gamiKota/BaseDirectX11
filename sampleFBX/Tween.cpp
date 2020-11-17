/**
 * @file Tween.cpp
 */


/**
 * @include
 */
#include "Tween.h"
#include "Frame.h"
#include "Transform.h"
#include "debugproc.h"
#include "System.h"


Tween::Tween() {
	m_name		= "tween";
	m_isTween	= E_TWEEN::NONE;
	m_result	= float3();
	m_start		= float3();
	m_end		= float3();
	m_nowTime	= 0.f;
	m_maxTime	= 0.f;
}


Tween::Tween(std::string name) : Object() {
	m_name		= "tween";
	m_isTween	= E_TWEEN::NONE;
	m_result	= float3();
	m_start		= float3();
	m_end		= float3();
	m_nowTime	= 0.f;
	m_maxTime	= 0.f;
}


void Tween::DOTween(float3 start, float3 end, float time) {

	if (m_isTween == E_TWEEN::NONE) {
		m_isTween = E_TWEEN::DO;

		m_result	= float3();
		m_start		= start;
		m_end		= end;
		m_nowTime	= 0.f;
		m_maxTime	= time;
	}
}


void Tween::Update() {

	if (m_isTween == E_TWEEN::END) {
		m_isTween = E_TWEEN::NONE;
	}
	else if (m_isTween == E_TWEEN::DO) {

		// éûä‘Çë´Ç∑
		m_nowTime += Frame::GetInstance().GetDeltaTime();

		// 0 Å` 1ÇÃîÕàÕÇ…ê≥ãKâª
		float time = m_nowTime / m_maxTime;

		m_result = float3::lerp3(m_start, m_end, time);

		if (time >= 1.f && m_isTween == E_TWEEN::DO) {	// ç°tweeníÜ
			m_result = m_end;
			m_isTween = E_TWEEN::END;
		}
	}
}


// EOF