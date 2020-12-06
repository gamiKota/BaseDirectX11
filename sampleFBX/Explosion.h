/**
 * @file Explosion.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


class GameObjectMesh;


/**
 * @class Explosion : inheritance Component
 */
class Explosion : public Component {
public:
	float m_time;	//!< �o�ߎ���

public:
	void Start();
	void Update();
};


// EOF