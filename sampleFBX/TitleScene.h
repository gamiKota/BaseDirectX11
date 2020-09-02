#pragma once

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Scene.h"


class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};


// EOF