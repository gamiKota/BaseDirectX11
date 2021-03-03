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
#include <DirectXMath.h>
#include "SceneManager.h"
#include "Transform.h"
#include "ShaderManager.h"


/**
 * @forward declaration
 */
class Component;


enum class E_OBJECT {
	EMPTY,
	UI,
	BACKGROUND,
	MODEL,
	MESH,
};


/**
 * @class GameObject : inheritance Object
 */
class GameObject : public Object {
private:
	static std::list<GameObject*>	m_listFinds;		//!< �������X�g�擾�p
	std::string						m_tag;				//!< �^�O
	E_OBJECT						m_eObject;			//!< �I�u�W�F�N�g�^�O

public:
	std::list<Component*>			m_listComponent;	//!< �I�u�W�F�N�g�����R���|�[�l���g���X�g
	Transform*						m_transform;		//!< �g�����X�t�H�[��

	E_VS		m_vs;
	E_PS		m_ps;

public:
	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObject(std::string name = "GameObject", std::string tag = "none", E_OBJECT obj = E_OBJECT::EMPTY);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~GameObject();

	/**
	 * @brief ��������
	 */
	virtual void Init();

	/**
	 * @�I������
	 */
	virtual void Uninit();

	/**
	 * @brief �X�V����
	 */
	virtual void Update();

	/**
	 * @brief �ŏI�X�V����
	 */
	virtual void LastUpdate();

	/**
	 * @brief �`�揈��
	 */
	virtual void Draw();

	/**
	 * @brief �Փˏ���
	 */
	virtual void OnCollision(GameObject* obj);

	/**
	 * @brief GetTag
	 */
	std::string GetTag() {
		return m_tag;
	}

	/**
	 * @brief GetEObject
	 */
	E_OBJECT GetEObject() {
		return m_eObject;
	}

	/**
	 * @brief Find
	 */
	static GameObject* Find(std::string name);

	/**
	 * @brief FindGameObjectWithTag
	 */
	static GameObject* FindGameObjectWithTag(std::string tag);

	/**
	 * @brief FindGameObjectsWithTag
	 */
	static std::list<GameObject*> FindGameObjectsWithTag(std::string tag);

	/**
	 * @brief �I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
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
	 * @brief �I�u�W�F�N�g�������Ă���R���|�[�l���g��ǉ�
	 * @template <class T>
	 * @return class T
	 */
	template<class T>
	T* AddComponent() {
		T* buff = new T();
		buff->m_gameObject = this;
		Transform* p = dynamic_cast<Transform*>(buff);
		if (p != nullptr) {
			buff->m_transform = p;
		}
		else {
			buff->m_transform = m_transform;
		}
		m_listComponent.push_back(buff);

		const type_info& id = typeid(*buff);
		std::string name = (std::string)id.name();
		buff->m_name = name.substr(6);

		buff->Awake();
		if (SceneManager::GetInstance().m_scene->GetGameUpdate()) {
			buff->Start();
		}
		return buff;
	}

	/**
	 * @brief �I�u�W�F�N�g�������Ă���R���|�[�l���g�̍폜
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