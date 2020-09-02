#pragma once

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Scene.h"


class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};


// EOF