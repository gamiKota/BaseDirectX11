/**
 * @file Tween.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "Data.h"


enum class E_TWEEN {
	NONE,	// ‘Ò‹@ó‘Ô
	DO,		// “®ì’†
	END,	// I‚í‚Á‚½uŠÔ
};


/**
 * @class Tween
 */
class Tween : Object {
public:
	E_TWEEN m_isTween;

	float3 m_result;
	float3 m_start;
	float3 m_end;
	float  m_nowTime;
	float  m_maxTime;

public:

	Tween();
	Tween(std::string name);

	void DOTween(float3 start, float3 end, float time = 0.f);

	void Update();

	float3 GetResult() {
		return m_result;
	}
};


// EOF