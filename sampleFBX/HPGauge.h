#pragma once
#include "Component.h"
class GameObject;
class HPGauge : public Component {
public:
	void Start();
	void Update();
	void LastUpdate();

public:
	GameObject* m_pair;	//!< 表示される自信のキャラクター
};

// EOF