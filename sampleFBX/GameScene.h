#pragma once

//*****************************************************************************
// �C���N���[�h��
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