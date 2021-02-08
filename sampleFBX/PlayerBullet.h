#pragma once
#include "Component.h"

class GameObjectUI;
class GameObjectText;

enum class E_BULLET {
	NORMAL,
	LASER,
};

class PlayerBullet : public Component {
public:
	//GameObjectUI* m_ui;
	GameObjectText* m_text;

	int m_numBullet;

public:
	void Start();
	void Update();
	void shot(E_BULLET eBullet = E_BULLET::NORMAL);
};

// EOF