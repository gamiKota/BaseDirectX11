// 一人称カメラ [FPCamera.h]
#pragma once
#include "Camera.h"

class FPCamera : public CCamera
{
public:
	void Init();
	void Uninit();
	void Update();
};
