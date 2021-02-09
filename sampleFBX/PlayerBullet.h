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
	GameObjectUI* m_weapon;

	int m_numBullet;
	bool m_isReload;
	float m_isReloadTime;

	float m_errorTime;
	bool m_isError;

public:
	void Start();
	void Update();
	void shot(E_BULLET eBullet = E_BULLET::NORMAL);
};

// EOF