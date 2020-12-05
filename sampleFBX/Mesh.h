//=============================================================================
//
// メッシュ共通処理 [mesh.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once


#include <DirectXMath.h>
#include "D3DClass.h"
#include "Component.h"


//*****************************************************************************
// 定数定義
//*****************************************************************************
enum ePrimitiveType {
	PT_UNDEFINED = 0,
	PT_POINT,
	PT_LINE,
	PT_LINESTRIP,
	PT_TRIANGLE,
	PT_TRIANGLESTRIP,

	MAX_PRIMITIVETYPE
};

//**************************************
// 構造体定義
//**************************************
typedef struct {
	DirectX::XMFLOAT4	Diffuse;		// Diffuse color RGBA
	DirectX::XMFLOAT4	Ambient;		// Ambient color RGB
	DirectX::XMFLOAT4	Specular;		// Specular 'shininess'
	DirectX::XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;					// Sharpness if specular highlight
} MATERIAL;


class Material : public Component {
public:
	DirectX::XMFLOAT4	m_diffuse;		//!< Diffuse color RGBA
	DirectX::XMFLOAT4	m_ambient;		//!< Ambient color RGB
	DirectX::XMFLOAT4	m_specular;		//!< Specular 'shininess'
	DirectX::XMFLOAT4	m_emissive;		//!< Emissive color RGB
	float				m_power;		//!< Sharpness if specular highlight

public:
	Material();
	void SetImGuiVal();

public:
};


typedef struct {
	ID3D11Buffer* pVertexBuffer;			// 頂点バッファインターフェースへのポインタ
	ID3D11Buffer* pIndexBuffer;				// インデックスバッファインターフェースへのポインタ

	DirectX::XMFLOAT4X4 mtxWorld;			// ワールドマトリックス
	DirectX::XMFLOAT3 pos;					// ポリゴン表示位置の中心座標
	DirectX::XMFLOAT3 rot;					// ポリゴンの回転角
	int nNumVertex;							// 総頂点数	
	int nNumIndex;							// 総インデックス数

	DirectX::XMFLOAT4X4 mtxTexture;			// テクスチャ マトリックス
} MESH;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMesh(void);
void UninitMesh(void);
void UpdateMesh(MESH* pMesh);
void DrawMesh(MESH* pMesh, Material* material, ID3D11ShaderResourceView* texture);
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh, VERTEX_3D vertexWk[], int indexWk[]);
void ReleaseMesh(MESH* pMesh);
