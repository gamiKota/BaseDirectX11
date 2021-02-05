#pragma once
#include "Singleton.h"
class GameObjectUI;
class HPGauge;
class EnemyManager : public Singleton<EnemyManager> {
public:
	HPGauge* m_com;

public:
	void Initialize();
};

// EOF