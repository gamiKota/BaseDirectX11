//=============================================================================
//
// ビルボード弾 [bullet.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "MeshBullet.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "D3DClass.h"
#include "System.h"
//#include "shadow.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/bullet000.png"	// テクスチャファイル名

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BULLET			100

#define BULLET_SPEED		5.0f
#define BULLET_RADIUS		5.0f

#define MIN_FIELD_X			-320.0f
#define MAX_FIELD_X			320.0f
#define MIN_FIELD_Z			-320.0f
#define MAX_FIELD_Z			320.0f

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct {
	int			stat;		// 状態
	XMFLOAT3	pos;		// 位置
	XMFLOAT3	vel;		// 速度
} BULLET;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static MESH							g_mesh;					// 構造体
static MATERIAL						g_material;				// マテリアル

static BULLET						g_bullet[MAX_BULLET];	// ビルボード弾情報

// 初期化
void MeshBullet::Awake()
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();
	HRESULT hr = S_OK;
	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Load Texture Faild", L"MeshBullet", MB_OK);
	}

	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// 頂点情報の作成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];
	pVertexWk[0].vtx = XMFLOAT3(-BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3( BULLET_RADIUS,  BULLET_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3( BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[4];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;
}

// 終了処理
void MeshBullet::Uninit()
{
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (pBullet->stat) {
			//ReleaseShadow(pBullet->nShadow);
			//pBullet->nShadow = 0;
			pBullet->stat = 0;
		}
	}
	ReleaseMesh(&g_mesh);
}

// 更新
void MeshBullet::Update() {
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (!pBullet->stat) continue;
		// 位置を更新
		pBullet->pos.x += pBullet->vel.x;
		pBullet->pos.y += pBullet->vel.y;
		pBullet->pos.z += pBullet->vel.z;
		// 範囲チェック
		if (pBullet->pos.x < MIN_FIELD_X ||
			pBullet->pos.x > MAX_FIELD_X ||
			pBullet->pos.z < MIN_FIELD_Z ||
			pBullet->pos.z > MAX_FIELD_Z) {
			pBullet->stat = 0;
			// 丸影解放
			//ReleaseShadow(pBullet->nShadow);
			//pBullet->nShadow = 0;
			continue;
		}

		// 丸影移動
		//MoveShadow(pBullet->nShadow, pBullet->pos);
	}
}

// 描画
void MeshBullet::Draw()
{
	ID3D11DeviceContext* pDC = D3DClass::GetInstance().GetDeviceContext();
	
	D3DClass::GetInstance().SetBlendState(BS_ALPHABLEND);
	//SetBlendState(BS_ALPHABLEND);

	XMFLOAT4X4& mView = CCamera::Get()->GetView();
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (!pBullet->stat) continue;
		// ビュー行列の回転成分の転置行列を設定
		g_mesh.mtxWorld._11 = mView._11;
		g_mesh.mtxWorld._12 = mView._21;
		g_mesh.mtxWorld._13 = mView._31;
		g_mesh.mtxWorld._21 = mView._12;
		g_mesh.mtxWorld._22 = mView._22;
		g_mesh.mtxWorld._23 = mView._32;
		g_mesh.mtxWorld._31 = mView._13;
		g_mesh.mtxWorld._32 = mView._23;
		g_mesh.mtxWorld._33 = mView._33;
		// 位置を反映
		g_mesh.mtxWorld._41 = pBullet->pos.x;
		g_mesh.mtxWorld._42 = pBullet->pos.y;
		g_mesh.mtxWorld._43 = pBullet->pos.z;
		// 描画
		DrawMesh(pDC, &g_mesh);
	}

	D3DClass::GetInstance().SetBlendState(BS_NONE);
}

// 発射
int MeshBullet::Fire(XMFLOAT3 pos, XMFLOAT3 dir)
{
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < MAX_BULLET; ++i, ++pBullet) {
		if (pBullet->stat) continue;
		pBullet->pos = pos;
		pBullet->vel.x = dir.x * BULLET_SPEED;
		pBullet->vel.y = dir.y * BULLET_SPEED;
		pBullet->vel.z = dir.z * BULLET_SPEED;
		//pBullet->nShadow = CreateShadow(pos, BULLET_RADIUS);
		pBullet->stat = 1;
		return i;
	}
	return -1;
}


// EOF