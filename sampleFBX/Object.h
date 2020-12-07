/**
 * @file Object.h
 */


/**
 * @include guard
 */
#ifndef ___HEADER_OBJECT_H___
#define ___HEADER_OBJECT_H___


/**
 * @include
 */
#include <string>
#include "Data.h"


/**
 * @forward declaration
 */
class GameObject;


/**
 * @class Object
 */
class Object {
private:
	bool				m_isActive;			//!< アクティブ状態
	const unsigned int	m_ID;				//!< ID
	bool				m_isDontDestroy;	//!< シーンのロード

public:
	std::string			m_name;		//!< 名前

public:
	Object() : m_isDontDestroy(false), m_isActive(true), m_ID(unsigned int(this)), m_name("Object") {}
	Object(std::string name, bool active = true) : m_isDontDestroy(false), m_isActive(active), m_ID(unsigned int(this)), m_name(name) {}

	std::string ToString() { return m_name; }
	const unsigned int GetInstanceID() { return m_ID; }
	const bool GetActive() { return m_isActive; }
	bool isDintDestroy() { return m_isDontDestroy; }

	/**
	 * @brief Instantiate
	 * @details *この関数でオブジェクトを生成してからコンポーネントの追加すること
	 */
	static GameObject* Instantiate(GameObject* obj, float3 transform = float3(), Quaternion rotate = Quaternion(), float3 scale = float3(1.f, 1.f, 1.f));

	/**
	 : @brief Destroy
	 */
	static void Destroy(GameObject* obj);

	/**
	 * @brief DontDestroyOnLoad
	 */
	static void DontDestroyOnLoad(GameObject* obj);


	bool operator == (Object object) {
		return this->m_ID == object.m_ID ? true : false;
	}
	bool operator != (Object object) {
		return !(*this == object);
	}
};


#endif // !___HEADER_OBJECT_H___
// EOF