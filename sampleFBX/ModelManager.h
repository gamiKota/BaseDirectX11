/**
 * @file ModelManager.h
 */


/**
 * @include guard
 */
#ifndef ___HEADER_MODELMANAGER_H___
#define ___HEADER_MODELMANAGER_H___


/*
 * @include
 */
#include "Singleton.h"
#include <DirectXMath.h>


class CFbxModel;


/**
 * @enum E_MODEL
 */
enum E_MODEL {
	E_MODEL_NONE,
	E_MODEL_PLAYER,
	E_MODEL_ENEMY,
	E_MODEL_BULLET,
	E_MODEL_LAND,
	E_MODEL_SKY,
	E_MODEL_MAX,
};


class GameObject3D;


/**
 * @class ModelManagerÅ@: inheritance Singleton
 */
class ModelManager : public Singleton<ModelManager> {
public:
	ModelManager();
	~ModelManager();
	void Init();
	void Uninit();
	CFbxModel* Get(E_MODEL model);

	void Update(GameObject3D* obj);
	void Draw(GameObject3D* obj);

private:
	CFbxModel* m_pModel[E_MODEL_MAX];
};


#endif // !___HEADER_MODELMANAGER_H___

 // EOF