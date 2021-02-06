#pragma once
#include "Component.h"

enum class E_BULLET {
	NORMAL,
	LASER,
};

class PlayerBullet : public Component {
public:
	void Start();
	void Update();
	void shot();
};

// EOF