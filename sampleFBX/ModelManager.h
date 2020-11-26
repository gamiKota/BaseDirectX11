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
	CFbxModel* Get(E_MODEL model);

	void Update(E_MODEL model);
	void Draw(E_MODEL model, DirectX::XMFLOAT4X4 transform);

private:
	CFbxModel* m_pModel[E_MODEL_MAX];
};


#endif // !___HEADER_MODELMANAGER_H___

 // EOF