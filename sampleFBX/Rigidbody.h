/**
 * @file Rigidbody.h
 * @brief 物理エンジン機能
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
	_5,		// 他に絶対影響されない


	_WALL,	// 壁
	_LAND,	// 壁
};


/**
 * @class Rigidbody : inheritance Component
 */
class Collider : public Component {
public:
	E_WEIGHT m_weight;	//!< 重さ

public:
	Collider();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();

	static void ShiftCollision(GameObject* obj1, GameObject* obj2);

private:
};


// EOF