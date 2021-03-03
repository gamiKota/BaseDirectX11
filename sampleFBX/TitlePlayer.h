#pragma once
#include "Component.h"
#include <vector>


class TitlePlayer : public Component {
private:
	float m_time;

	std::vector<GameObject*> m_obj;

public:
	void Start();
	void Update();

	static bool m_isCreate;
};

