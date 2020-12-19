/**
 * @file WaterSurface.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "GameObjectMeshBase.h"
#include "TextureManager.h"
#include "Mesh.h"


#define MAX_DROP 256


class Drop;


/**
 * @struct WaterSurfaceParam
 */
typedef struct {

	// 実際にシェーダに与えられるパラメータ
	float m_pos[2];		//!< UV座標(x, y)
	float m_time;		//!< 経過時間(z)
	float m_influence;	//!< 影響力(w)

	// パラメータに働く補正値
	float m_influenceSub;	//!< 影響力の減算パラメータ

} WaterSurfaceParam;


/**
 * @class WaterSurface : inheritance GameObjectMeshBase
 */
class WaterSurface : public GameObjectMeshBase {
public:
	MESH				m_mesh;					//!< メッシュ
	//WaterSurfaceParam*	m_dropList[MAX_DROP];	//!< 雫(可変長はちょっと厳しい)
	Drop*				m_dropList[MAX_DROP];	//!< 雫
	float				m_dropTime;				//!< 雫生成時間
	//std::list<Drop*>	m_dropList;	//!< 雫

public:
	/**
	 * @brief コンストラクタ
	 */
	WaterSurface();

	/**
	 * @brief コンストラクタ
	 * @param[in] name
	 * @param[in] tag
	 */
	WaterSurface(std::string name = "GameObjectUI", std::string tag = "none");

	/**
	 * @brief デストラクタ
	 */
	virtual ~WaterSurface();

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


	void SetImGuiVal();
};


// EOF