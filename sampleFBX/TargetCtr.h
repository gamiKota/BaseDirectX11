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
 * @class Bullet : inheritance Character
 */
class TargetCtr : public Component {
public:
	GameObject* m_parent = nullptr;	//!< �e
	GameObject* m_target = nullptr;	//!< �^�[�Q�b�g

public:
	void Start();
	void Update();
};


float3 LockOnMarker(float3 target);


// EOF