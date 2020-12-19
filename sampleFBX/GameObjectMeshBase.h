/**
 * @file GameObjectMeshBase.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "GameObject.h"


/**
 * @class GameObjectMeshBase
 */
class GameObjectMeshBase : public GameObject {
public:
	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	GameObjectMeshBase(std::string name = "GameObjectMesh", std::string tag = "none");
};

// EOF