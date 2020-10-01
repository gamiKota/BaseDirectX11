#pragma once

#include "Singleton.h"
#include "Scene.h"

class GameObject;

enum class E_SCENE {
	TITLE,
	GAME,
	RESULT,
};

class SceneManager : public Singleton<SceneManager> {
public:
	std::list<GameObject*> m_DontDestroyObject;

public:
	Scene* m_scene;
	bool m_isChange;
	E_SCENE m_nowScene;

public:
	SceneManager();
	~SceneManager();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void LoadScene(E_SCENE scene);
};


// EOF