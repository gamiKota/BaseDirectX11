// 境界ボックス デバッグ表示 [Box.h]
#pragma once
#include "main.h"
#include "Component.h"
#include "ModelManager.h"

class Collision : public Component {
public:
	E_MODEL		m_model;
	XMFLOAT4	m_color;
	XMFLOAT3	m_vCenter;		// 境界ボックス中心座標
	XMFLOAT3	m_vBBox;		// 境界ボックス サイズ
	XMFLOAT3	m_vPosBBox;		// 境界ボックス中心座標(ワールド空間)
	XMFLOAT4X4	m_world;		//!< 当たり判定用行列
	bool		m_bHit;			// 衝突有無

	ID3D11Buffer* m_pVertexBuffer;			// 頂点バッファ
	ID3D11Buffer* m_pConstantBuffer[2];		// 定数バッファ
	ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダ
	ID3D11InputLayout* m_pInputLayout;		// 頂点フォーマット
	ID3D11PixelShader* m_pPixelShader;		// ピクセルシェーダ
	ID3D11Buffer* m_pIndexBuffer;			// インデックスバッファ				// インデックスバッファインターフェースへのポインタ

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void DebugDraw();

	XMFLOAT4& GetColor() { return m_color; }
	void SetColor(XMFLOAT4* pColor) { m_color = *pColor; }

	void SetWorld(XMFLOAT4X4 world);

	void Init(E_MODEL model);
	static bool AABB(Collision obj1, Collision obj2);
};