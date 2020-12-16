/**
 * @file ggCamera.h
 */


#pragma once


#include "Camera.h"
#include <Windows.h>


class ggCamera : public CCamera
{
protected:
	POINT m_oldMousePos;

public:
	void Awake();
	void Start();
	void Update();
	void LastUpdate();
};


// EOF