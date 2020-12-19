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
	float m_time;		//!< —Ž‰ºŽžŠÔ
	float m_influence;	//!< ‰e‹¿—Í
	bool  m_isCollsion;	//!< ”g‚Æ“–‚½‚Á‚½‚©‚Ç‚¤‚©

public:

	void Start();
	void Update();
	void Uninit();

	void OnCollision(GameObject* obj);
};


// EOF