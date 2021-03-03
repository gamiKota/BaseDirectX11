#pragma once
#include "Component.h"
class TitleManager : public Component {
private:
	float m_time;
	float m_isLogo;

public:
	void Start();
	void Update();
};

