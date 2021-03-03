#pragma once
#include "Camera.h"


class TitleCamera : public CCamera {
private:
	GameObject* m_player;
public:
	void Start();
	void Update();
};