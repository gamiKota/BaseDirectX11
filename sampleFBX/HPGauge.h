#pragma once
#include "Component.h"
class GameObject;
class HPGauge : public Component {
public:
	float m_HP;
	float m_maxHP;

public:
	void Start();
	void Update();
	void LastUpdate();

	void SetHP(float HP, float maxHP) {
		m_HP = HP;
		m_maxHP = maxHP;
	}
};

// EOF