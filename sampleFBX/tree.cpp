//=============================================================================
//
// 木処理 [tree.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "tree.h"
#include "camera.h"
//#include "shadow.h"
#include "light.h"
#include "Texture.h"
#include "Mesh.h"
#include "D3DClass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TREE			L"data/Texture/tree000.png"		// 読み込むテクスチャファイル名
#define	TREE_WIDTH				(50.0f)							// 弾の半径幅
#define	TREE_HEIGHT				(80.0f)							// 弾の半径幅

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTree(ID3D11Device* pDevice);
void SetVertexTree(int idxTree, float width, float height);
void SetColorTree(int idxTree, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_meshTree;						// メッシュ情報
static MATERIAL			g_material;
static TREE				g_aTree[MAX_TREE];				// 木ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTree(void)
{
	ID3D11Device* pDevice = D3DClass::GetInstance().GetDevice();

	// 頂点情報の作成
	MakeVertexTree(pDevice);

	// テクスチャの読み込み
	CreateTextureFromFile(pDevice,					// デバイスへのポインタ
						  TEXTURE_TREE,				// ファイルの名前
						  &g_meshTree.pTexture);	// 読み込むメモリー
	XMStoreFloat4x4(&g_meshTree.mtxTexture, XMMatrixIdentity());

	g_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_material.Power = 0.0f;
	g_meshTree.pMaterial = &g_material;

	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		g_aTree[cntTree].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aTree[cntTree].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTree[cntTree].width = TREE_WIDTH;
		g_aTree[cntTree].height = TREE_HEIGHT;
		g_aTree[cntTree].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTree(void)
{
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			//DeleteShadow(g_aTree[cntTree].idxShadow);
			g_aTree[cntTree].idxShadow = -1;
			g_aTree[cntTree].bUse = false;
		}
	}
	// メッシュの開放
	ReleaseMesh(&g_meshTree);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTree(void)
{
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			// 影の位置設定
			//SetPositionShadow(g_aTree[cntTree].idxShadow, XMFLOAT3(g_aTree[cntTree].pos.x, 0.1f, g_aTree[cntTree].pos.z));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTree(void)
{
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ライティング無効
	SetLightEnable(false);

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetView();
	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (g_aTree[cntTree].bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshTree.mtxWorld, mtxWorld);

			g_meshTree.mtxWorld._11 = mtxView._11;
			g_meshTree.mtxWorld._12 = mtxView._21;
			g_meshTree.mtxWorld._13 = mtxView._31;
			g_meshTree.mtxWorld._21 = mtxView._12;
			g_meshTree.mtxWorld._22 = mtxView._22;
			g_meshTree.mtxWorld._23 = mtxView._32;
			g_meshTree.mtxWorld._31 = mtxView._13;
			g_meshTree.mtxWorld._32 = mtxView._23;
			g_meshTree.mtxWorld._33 = mtxView._33;
			mtxWorld = XMLoadFloat4x4(&g_meshTree.mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(g_aTree[cntTree].width, g_aTree[cntTree].height, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aTree[cntTree].pos.x, g_aTree[cntTree].pos.y, g_aTree[cntTree].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&g_meshTree.mtxWorld, mtxWorld);

			// 色の設定
			g_material.Diffuse = g_aTree[cntTree].col;

			// ポリゴンの描画
			DrawMesh(pDeviceContext, &g_meshTree);
		}
	}

	// ライティング有効
	SetLightEnable(true);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexTree(ID3D11Device* pDevice)
{
	// 一時的な頂点配列を生成
	g_meshTree.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshTree.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2.0f, 1.0f, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2.0f, 0.0f, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2.0f, 1.0f, 0.0f);

	// 法線の設定
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	g_meshTree.nNumIndex = 4;
	int* pIndexWk = new int[g_meshTree.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	HRESULT hr = MakeMeshVertex(pDevice, &g_meshTree, pVertexWk, pIndexWk);

	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTree(int idxTree, float width, float height)
{
	if (idxTree >= 0 && idxTree < MAX_TREE) {
		g_aTree[idxTree].width = width;
		g_aTree[idxTree].height = height;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTree(int idxTree, XMFLOAT4 col)
{
	if (idxTree >= 0 && idxTree < MAX_TREE) {
		g_aTree[idxTree].col = col;
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetTree(XMFLOAT3 pos, float width, float height, XMFLOAT4 col)
{
	int idxTree = -1;

	for (int cntTree = 0; cntTree < MAX_TREE; ++cntTree) {
		if (!g_aTree[cntTree].bUse) {
			g_aTree[cntTree].pos = pos;
			g_aTree[cntTree].bUse = true;

			// 頂点座標の設定
			SetVertexTree(cntTree, width, height);

			// 頂点カラーの設定
			SetColorTree(cntTree, col);

			// 影の設定
			//g_aTree[cntTree].idxShadow = SetShadow(g_aTree[cntTree].pos, g_aTree[cntTree].width, g_aTree[cntTree].width);

			idxTree = cntTree;
			break;
		}
	}

	return idxTree;
}

//=============================================================================
// 木を取得
//=============================================================================
TREE *GetTree(void)
{
	return &g_aTree[0];
}
