#pragma once


#include "Scene.h"


class ResultScene : public Scene {
public:
	ResultScene();
	~ResultScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

// EOF