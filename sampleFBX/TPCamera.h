/**
 * @file TPCamera.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Camera.h"


/**
 * @class TPCamera : inheritance CCamera
 */
class TPCamera : public CCamera
{
protected:
	GameObject* m_player;

public:
	void Awake();
	void Start();
	void Uninit();
	void LastUpdate();
};


// EOF