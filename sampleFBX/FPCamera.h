// 一人称カメラ [FPCamera.h]
#pragma once
#include "Camera.h"

class CFPCamera : public CCamera
{
public:
	HRESULT Init();
	void Uninit();
	void Update();
};
