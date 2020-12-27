/**
 * @file TargetCtr.h
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
 * @class TargetCtr : inheritance Component
 */
class TargetCtr : public Component {
public:
	GameObject* m_parent;	//!< �e
	GameObject* m_target;	//!< �^�[�Q�b�g
	float m_colY;

public:
	void Start();
	void Update();
};


float3 LockOnMarker(float3 target);


// EOF