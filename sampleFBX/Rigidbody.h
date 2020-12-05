/**
 * @file Rigidbody.h
 * @brief �����G���W���@�\
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


/**
 * @enum E_WEIGHT
 */
enum class E_WEIGHT {
	_0 = 0,
	_1,
	_2,
	_3,
	_4,
	_5,		// ���ɐ�Ήe������Ȃ�


	_WALL,	// ��
};


/**
 * @class Rigidbody : inheritance Component
 */
class Rigidbody : public Component {
public:
	E_WEIGHT m_weight;	//!< �d��

public:
	Rigidbody();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();

	static void ShiftCollision(GameObject* obj1, GameObject* obj2);

private:
};


// EOF