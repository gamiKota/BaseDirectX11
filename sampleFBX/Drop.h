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
	float m_time;		//!< ��������
	float m_influence;	//!< �e����
	bool  m_isCollsion;	//!< �g�Ɠ����������ǂ���

public:

	void Start();
	void Update();
	void Uninit();

	void OnCollision(GameObject* obj);
};


// EOF