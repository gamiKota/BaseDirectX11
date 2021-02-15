#pragma once

#include "Component.h"
#include <functional>

class AI : public Component {
public:

	AI* StartUp(float waitTime = 2.f);

	void OnComplete(std::function<void()> func) { m_comp = func; }

	std::function<void()> m_comp;
};