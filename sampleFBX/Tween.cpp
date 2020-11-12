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



Tween::Tween(std::string name) : Object(name) {
	//m_name		= name;

	m_isTween	= E_TWEEN::NONE;
	m_result	= float3();
	m_start		= float3();
	m_end		= float3();
	m_nowTime	= 0.f;
	m_maxTime	= 0.f;
}


Tween* Tween::DOTween(float3 start, float3 end, float time) {

	if (m_isTween == E_TWEEN::NONE) {
		m_isTween = E_TWEEN::DO;

		m_result	= float3();
		m_start		= start;
		m_end		= end;
		m_nowTime	= 0.f;
		m_maxTime	= time;
	}
	return this;
}


void Tween::Update() {

	if (m_isTween == E_TWEEN::NONE) {

		PrintDebugProc("start = %.2f, %.2f, %.2f\n", m_start.x, m_start.y, m_start.z);
		PrintDebugProc("end = %.2f, %.2f, %.2f\n", m_end.x, m_end.y, m_end.z);
		PrintDebugProc("result = %.2f, %.2f, %.2f\n", m_result.x, m_result.y, m_result.z);

		PrintDebugProc("m_nowTime = %.2f\n", m_nowTime);
		PrintDebugProc("m_nowTime = %.2f\n", m_maxTime);

		return;
	}

	if (m_isTween == E_TWEEN::DO) {

		// éûä‘Çë´Ç∑
		m_nowTime += Frame::GetInstance().GetDeltaTime();

		// 0 Å` 1ÇÃîÕàÕÇ…ê≥ãKâª
		float time = m_nowTime / m_maxTime;

		m_result = float3::lerp3(m_start, m_end, time);

		if (time >= 1.f && m_isTween == E_TWEEN::DO) {	// ç°tweeníÜ
		// èIÇÌÇË
			m_isTween = E_TWEEN::NONE;
			return;
		}
	}

	PrintDebugProc("start = %.2f, %.2f, %.2f\n", m_start.x, m_start.y, m_start.z);
	PrintDebugProc("end = %.2f, %.2f, %.2f\n", m_end.x, m_end.y, m_end.z);
	PrintDebugProc("result = %.2f, %.2f, %.2f\n", m_result.x, m_result.y, m_result.z);

	PrintDebugProc("m_nowTime = %.2f\n", m_nowTime);
	PrintDebugProc("m_nowTime = %.2f\n", m_maxTime);
}


// EOF