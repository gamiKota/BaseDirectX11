// 三人称カメラ [TPCamera.h]
#pragma once
#include "Camera.h"

class TPCamera : public CCamera
{
public:
	void Awake();
	void Start();
	void Uninit();
	void Update();
};
