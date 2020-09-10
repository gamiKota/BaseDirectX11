// OlÌƒJƒƒ‰ [TPCamera.h]
#pragma once
#include "Camera.h"

class TPCamera : public CCamera
{
protected:
	GameObject* m_player;

public:
	void Awake();
	void Start();
	void Uninit();
	void Update();
};
