#pragma once
#include "Component.h"
class GameObjectUI;
class PlayerHP : public Component {
private:
	GameObjectUI* m_hp;
	GameObjectUI* m_logo;

public:
	void Start();
	void Update();
};

// EOF