#pragma once
#include "Component.h"

class GameObjectUI;

enum class E_BULLET {
	NORMAL,
	LASER,
};

class PlayerBullet : public Component {
public:
	GameObjectUI* m_ui;

public:
	void Start();
	void Update();
	void shot(E_BULLET eBullet = E_BULLET::NORMAL);
};

// EOF