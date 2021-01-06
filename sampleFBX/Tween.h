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
#include <Windows.h>
#include <iostream>
#include <functional>


enum class E_TWEEN {
	NONE,	// 待機状態
	DO,		// 動作中
	END,	// 終わった瞬間
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

	void Update();
	float3 GetResult() { return m_result; }


	void DOTween(float3 start, float3 end, float time = 0.f);

	/**
	 * @brief Tweenの終了時に呼ばれるコールバック関数の定義
	 * @param 関数ポインタ(戻り値なし、引数なし)
	 * @return 戻り値なし
	 */
	void OnComplete(std::function<void()> func) {
		m_onCompleteCallBuck = func;
	}

	std::function<void()> m_onCompleteCallBuck;
};


// EOF