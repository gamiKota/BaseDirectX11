// 三人称カメラ [TPCamera.h]
#pragma once
#include "Camera.h"

class TPCamera : public CCamera
{
public:
	void Init();
	void Uninit();
	void Update();
};
