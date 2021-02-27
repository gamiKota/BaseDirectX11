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
#include <d3d11.h>


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
private:
	ID3D11ShaderResourceView* m_pRTTex;
	ID3D11RenderTargetView* m_pRTView;
	ID3D11DepthStencilView* m_pDSView;
	int m_step;

protected:
	bool			m_isUpdate;
	std::string		m_name;		//!< ���݃A�N�e�B�u�ȃV�[���̖��O
	GameObject*		m_empty;	//!< ��̃I�u�W�F�N�g
	GameObject3D*	m_object3D;	//!< 3D�I�u�W�F�N�g
	GameObjectUI*	m_UI;		//!< UI
	GameObjectMesh*	m_mesh;		//!< ���b�V��

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