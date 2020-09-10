/**
 * @file Object.h
 */


/**
 * @include guard
 */
#ifndef ___HEADER_GAMEOBJECT_H___
#define ___HEADER_GAMEOBJECT_H___


/**
 * @include
 */
#include "Object.h"
#include <list>
#include "main.h"
#include <DirectXMath.h>
#include "SceneManager.h"
#include "Transform.h"


/**
 * @forward declaration
 */
class Component;


/**
 * @class GameObject : inheritance Object
 */
class GameObject : public Object {
private:
	static std::list<GameObject*>	m_listFinds;		//!< 複数リスト取得用
	std::string						m_tag;				//!< タグ

public:
	std::list<Component*>			m_listComponent;	//!< オブジェクトが持つコンポーネントリスト
	Transform*						m_transform;		//!< トランスフォーム

public:
	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObject(std::string name = "GameObject", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~GameObject();

	/**
	 * @brief 初期処理
	 */
	virtual void Init();

	/**
	 * @終了処理
	 */
	virtual void Uninit();

	/**
	 * @brief 更新処理
	 */
	virtual void Update();

	/**
	 * @brief 最終更新処理
	 */
	virtual void LastUpdate();

	/**
	 * @brief 描画処理
	 */
	virtual void Draw();

	/**
	 * @brief 衝突処理
	 */
	virtual void OnCollision(GameObject* obj);

	/**
	 * @brief GetTag
	 */
	std::string GetTag() {
		return m_tag;
	}

	/**
	 * @brief Find
	 */
	static GameObject* Find(std::string name);

	/**
	 * @brief FindTag
	 */
	static GameObject* FindGameObjectWithTag(std::string tag);

	/**
	 * @brief FindTags
	 */
	static std::list<GameObject*> FindGameObjectsWithTag(std::string tag);

	/**
	 * @brief Instance
	 */
	static void Instance(GameObject* obj, DirectX::XMFLOAT3 transform = DirectX::XMFLOAT3());

	/**
	 * @brief オブジェクトが持っているコンポーネントを取得
	 * @template <class T>
	 * @return class T or nullptr
	 */
	template<class T>
	T* GetComponent() {
		for (auto com : m_listComponent) {
			T* buff = dynamic_cast<T*>(com);
			if (buff != nullptr)
				return buff;
		}
		return nullptr;
	}

	/**
	 * @brief オブジェクトが持っているコンポーネントを追加
	 * @template <class T>
	 * @return class T
	 */
	template<class T>
	T* AddComponent() {
		T* buff = new T();
		buff->m_gameObject = this;
		buff->m_transform = m_transform;
		m_listComponent.push_back(buff);
		buff->Awake();
		if (SceneManager::GetInstance().m_scene->GetGameUpdate()) {
			buff->Start();
		}
		return buff;
	}

	/**
	 * @brief オブジェクトが持っているコンポーネントの削除
	 * @template <class T>
	 */
	template<class T>
	void RemoveComponent() {
		auto it = m_listComponent;
		for (auto com : it) {
			T* buff = dynamic_cast<T*>(com);
			if (buff != nullptr) {
				m_listComponent.remove(com);
				delete com;
			}
		}
	}
};

#endif // !___HEADER_GAMEOBJECT_H___
// EOF