#pragma once

#include "Component.h"
#include <functional>
#include <vector>
#include <map>

class AI : public Component {
private:
	float m_time;
	float m_maxTime;
	bool m_lock;
	bool m_isStart;

public:
	AI();
	void Update();
	void Init();

	AI* StartUp(float waitTime = 2.f, bool lock = false);

	void OnStart(std::function<void()> func) { m_start = func; }
	std::function<void()> m_start;
	void OnComplete(std::function<void()> func) { m_comp = func; }
	std::function<void()> m_comp;

	std::map<int, std::function<void()>> m_table;
	//std::function<AI*()> m_table;
};