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
#include <vector>


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


struct INSTANCING_PARAMETER {
	DirectX::XMMATRIX mWorld;

	INSTANCING_PARAMETER() {
		mWorld = DirectX::XMMatrixIdentity();
	}
};

struct INSTANCING_DATA {
	INSTANCING_PARAMETER paramter[50];
	int num;

	INSTANCING_DATA() {
		for (auto p : paramter) {
			p = INSTANCING_PARAMETER();
		}
		num = 0;
	}
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

	FBXPlayer* Get(E_MODEL model);
	DrawBuffer* GetBuf(E_MODEL model);

	void Update(E_MODEL model);
	void Draw(E_MODEL model);
	void DrawInstanced(E_MODEL model);

	void SetInstancingParamter(INSTANCING_PARAMETER ip);

private:
	FBXPlayer* m_pModelData[E_MODEL_MAX];
	DrawBuffer* m_pModelBuf[E_MODEL_MAX];

	DrawBuffer* m_pShadow;

	INSTANCING_DATA m_instancingData;
};


#endif // !___HEADER_MODELMANAGER_H___

 // EOF