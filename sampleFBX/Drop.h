/**
 * @file Drop.h
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
 * @class Drop : inheritance Component
 */
class Drop : public Component {
public:
	float m_time;		//!< 落下時間
	float m_influence;	//!< 影響力
	bool  m_isCollsion;	//!< 波と当たったかどうか

public:

	void Start();
	void Update();
	void Uninit();

	void OnCollision(GameObject* obj);
};


// EOF