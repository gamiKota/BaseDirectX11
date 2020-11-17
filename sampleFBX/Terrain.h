// 地面クラス [Land.h]
#pragma once
#include "main.h"
#include "FbxModel.h"

class CLand
{
private:
	CFbxModel*	m_pModel;	// モデル

	TFbxVertex*	m_pVertex;	// 頂点配列
	int			m_nVertex;	// 頂点数
	int*		m_pIndex;	// インデックス配列
	int			m_nIndex;	// インデックス数

public:
	CLand();
	~CLand();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool Collision(XMFLOAT3* pP0, XMFLOAT3* pW,
		XMFLOAT3* pX, XMFLOAT3* pN = nullptr);
};
