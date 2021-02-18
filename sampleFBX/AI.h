#pragma once

#include "Component.h"
#include <functional>

class AI : public Component {
private:
	float m_time;
	float m_maxTime;
	bool m_lock;

public:
	AI();
	void Update();
	void Init();

	AI* StartUp(float waitTime = 2.f, bool lock = false);

	void OnComplete(std::function<void()> func) { m_comp = func; }

	std::function<void()> m_comp;
};