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


class PlayerMgr;


/**
 * @class TPCamera : inheritance CCamera
 */
class TPCamera : public CCamera
{
protected:
	PlayerMgr* m_player;

public:
	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
};


// EOF