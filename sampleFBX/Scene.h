/**
 * @file Scene.h
 */


/**
 * @include guard
 */
#ifndef ___HEADER_SCENE_H___
#define ___HEADER_SCENE_H___


/**
 * @include
 */
#include <list>

class GameObject;


/**
 * @class Scene
 */
class Scene {
protected:
	bool m_isUpdate;
	std::string m_name;

public:
	std::list<GameObject*> m_listObject;	//!< オブジェクトリスト
	Scene();
	~Scene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Shutdown();

	bool GetGameUpdate() {
		return m_isUpdate;
	}
	const char* GetName() {
		return m_name.c_str();
	}
};


// shared_ptr<>使え


#endif // !___HEADER_SCENE_H___
// EOF