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
	GameObject* m_parent;	//!< 親
	GameObject* m_target;	//!< ターゲット
	float m_colY;

public:
	void Start();
	void Update();
};


float3 LockOnMarker(float3 target);


// EOF