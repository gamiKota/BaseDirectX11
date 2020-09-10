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
	static std::list<GameObject*>	m_listFinds;		//!< �������X�g�擾�p
	std::string						m_tag;				//!< �^�O

public:
	std::list<Component*>			m_listComponent;	//!< �I�u�W�F�N�g�����R���|�[�l���g���X�g
	Transform*						m_transform;		//!< �g�����X�t�H�[��

public:
	/**
	 * @brief �R���X�g���N�^
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObject(std::string name = "GameObject", std::string tag = "none");

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
		buff->m_transform = m_transform;
		m_listComponent.push_back(buff);
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