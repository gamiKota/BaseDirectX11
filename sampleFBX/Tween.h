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
	NONE,	// �ҋ@���
	DO,		// ���쒆
	END,	// �I������u��
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
	 * @brief Tween�̏I�����ɌĂ΂��R�[���o�b�N�֐��̒�`
	 * @param �֐��|�C���^(�߂�l�Ȃ��A�����Ȃ�)
	 * @return �߂�l�Ȃ�
	 */
	void OnComplete(std::function<void()> func) {
		m_onCompleteCallBuck = func;
	}

	std::function<void()> m_onCompleteCallBuck;
};


// EOF