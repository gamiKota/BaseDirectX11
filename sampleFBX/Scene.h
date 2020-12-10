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


/**
 * @forward declaration
 */
class GameObject;
class GameObject3D;
class GameObjectUI;
class GameObjectMesh;


/**
 * @class Scene
 */
class Scene {
protected:
	bool			m_isUpdate;
	std::string		m_name;		//!< 現在アクティブなシーンの名前
	GameObject*		m_empty;	//!< 空のオブジェクト
	GameObject3D*	m_object3D;	//!< 3Dオブジェクト
	GameObjectUI*	m_UI;		//!< UI
	GameObjectMesh*	m_mesh;		//!< メッシュ

public:
	std::list<GameObject*> m_listObject;	//!< オブジェクトリスト
	Scene();
	~Scene();
	virtual void Init();
	void Uninit();
	void Update();
	void Draw();

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