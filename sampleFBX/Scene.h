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


/**
 * @class Scene
 */
class Scene {
protected:
	bool			m_isUpdate;
	std::string		m_name;
	GameObject*		m_empty;	//!< ��̃I�u�W�F�N�g
	GameObject3D*	m_object3D;	//!< 3D�I�u�W�F�N�g
	GameObjectUI*	m_UI;		//!< ��荇����
	GameObjectUI*	m_number;	//!< ��荇����
	GameObjectUI*	m_Button;	//!< ��荇����

public:
	std::list<GameObject*> m_listObject;	//!< �I�u�W�F�N�g���X�g
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


// shared_ptr<>�g��


#endif // !___HEADER_SCENE_H___
// EOF