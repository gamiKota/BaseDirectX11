// 三人称カメラ [TPCamera.h]
#pragma once
#include "Camera.h"

class CTPCamera : public CCamera
{
public:
	HRESULT Init();
	void Uninit();
	void Update();
};
