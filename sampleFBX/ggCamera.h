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
	float3 m_vLook;
	float3 m_vUp;

public:
	ggCamera();
	void Awake();
	void Start();
	void Update();
	void LastUpdate();
};


// EOF