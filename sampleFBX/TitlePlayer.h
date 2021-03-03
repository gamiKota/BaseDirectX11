#pragma once
#include "Component.h"
class TitlePlayer : public Component {
private:
	float m_time;
public:
	void Start();
	void Update();

	static bool m_isCreate;
};

