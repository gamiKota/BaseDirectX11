/**
 * @file Collision.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "ModelManager.h"
#include <d3d11.h>


/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	E_MODEL				m_model;
	DirectX::XMFLOAT4	m_color;
	DirectX::XMFLOAT3	m_vCenter;		//!< 境界ボックス中心座標
	DirectX::XMFLOAT3	m_vBBox;		//!< 境界ボックス サイズ
	DirectX::XMFLOAT3	m_vPosBBox;		//!< 境界ボックス中心座標(ワールド空間)
	DirectX::XMFLOAT4X4	m_world;		//!< 当たり判定用行列
	bool				m_bHit;			//!< 衝突有無

	ID3D11Buffer* m_pVertexBuffer;			//!< 頂点バッファ
	ID3D11Buffer* m_pConstantBuffer[2];		//!< 定数バッファ
	ID3D11VertexShader* m_pVertexShader;	//!< 頂点シェーダ
	ID3D11InputLayout* m_pInputLayout;		//!< 頂点フォーマット
	ID3D11PixelShader* m_pPixelShader;		//!< ピクセルシェーダ
	ID3D11Buffer* m_pIndexBuffer;			//!< インデックスバッファ				// インデックスバッファインターフェースへのポインタ

private:
	bool m_isInit;

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void DebugDraw();

	DirectX::XMFLOAT4& GetColor() { return m_color; }
	void SetColor(DirectX::XMFLOAT4* pColor) { m_color = *pColor; }

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
	void Init(E_MODEL model);
	void SetWorld(DirectX::XMFLOAT4X4 world);
};


// EOF