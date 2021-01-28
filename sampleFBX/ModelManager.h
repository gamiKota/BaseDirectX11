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
class GameObject3D;
class FBXPlayer;
class DrawBuffer;


/**
 * @enum E_MODEL
 */
enum E_MODEL {
	E_MODEL_NONE,
	E_MODEL_PLAYER,
	E_MODEL_ENEMY,
	E_MODEL_BULLET,
	E_MODEL_LASER,
	E_MODEL_LAND,
	E_MODEL_SKY,
	E_MODEL_MAX,
};




/**
 * @class ModelManagerÅ@: inheritance Singleton
 */
class ModelManager : public Singleton<ModelManager> {
public:
	ModelManager();
	~ModelManager();
	void Init();
	void Uninit();

	void Update(E_MODEL model);
	void Draw(E_MODEL model);

private:

	FBXPlayer* m_pModelData[E_MODEL_MAX];
	DrawBuffer* m_pModelBuf[E_MODEL_MAX];
};


#endif // !___HEADER_MODELMANAGER_H___

 // EOF