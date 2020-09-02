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

	const char* ToString() { return m_name.c_str(); }
	const unsigned int GetInstanceID() { return m_ID; }
	const bool GetActive() { return m_isActive; }
	bool isDintDestroy() { return m_isDontDestroy; }

	/**
	 : @brief Destroy
	 */
	static void Destroy(GameObject* obj);

	/**
	 * @brief DontDestroyOnLoad
	 */
	static void DontDestroyOnLoad(GameObject* obj);
};


#endif // !___HEADER_OBJECT_H___
// EOF