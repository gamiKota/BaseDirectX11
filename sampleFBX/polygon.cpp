//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "polygon.h"
#include "Shader.h"
#include "Graphics.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "Geometory.h"
#include "System.h"



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexPolygon(ID3D11Device* pDevice);
static void SetVertexPolygon(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャへのポインタ

static VERTEX_2D					g_vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

static XMFLOAT3						g_posPolygon;			// ポリゴンの移動量
static XMFLOAT3						g_rotPolygon;			// ポリゴンの回転量
static XMFLOAT3						g_sizPolygon;			// ポリゴンのサイズ
static XMFLOAT4						g_colPolygon;			// ポリゴンの頂点カラー
static bool							g_bInvalidate;			// 頂点データ更新フラグ

static XMFLOAT2						g_posTexFrame;			// UV座標
static XMFLOAT2						g_sizTexFrame;			// テクスチャ抽出サイズ

static XMFLOAT4X4					g_mWorld;				// ワールド変換行列
static XMFLOAT4X4					g_mTex;					// テクスチャ変換行列


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygon(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;

	// 変換行列初期化
	XMStoreFloat4x4(&g_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mTex, XMMatrixIdentity());
	g_mTex._44 = 0.0f;

	g_posPolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotPolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizPolygon = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_colPolygon = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_bInvalidate = false;

	g_posTexFrame = XMFLOAT2(0.0f, 0.0f);
	g_sizTexFrame = XMFLOAT2(1.0f, 1.0f);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon(void)
{
	// (何もしない)
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(ID3D11DeviceContext* pDeviceContext)
{
	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(g_sizPolygon.x, g_sizPolygon.y, g_sizPolygon.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotPolygon.x),
		XMConvertToRadians(g_rotPolygon.y), XMConvertToRadians(g_rotPolygon.z));
	// 移動
	mWorld *= XMMatrixTranslation(g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&g_mWorld, mWorld);

	if (g_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(g_sizTexFrame.x, g_sizTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(g_posTexFrame.x, g_posTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&g_mTex, mWorld);
	} else {
		// テクスチャ無し
		g_mTex._44 = 0.0f;
	}

	ShaderManager* shader = &ShaderManager::GetInstance();

	SHADER_WORLD world;
	world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
	world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
	shader->UpdateBuffer("MainWorld", &world);

	shader->SetTexturePS(g_pTexture);

	DrawPolygon();
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void SetPolygonTexture(ID3D11ShaderResourceView* pTexture)
{
	g_pTexture = pTexture;
	g_mTex._44 = (g_pTexture) ? 1.0f : 0.0f;
}

//=============================================================================
// 表示座標の設定
//=============================================================================
void SetPolygonPos(float fX, float fY)
{
	g_posPolygon.x = fX;
	g_posPolygon.y = fY;
}

//=============================================================================
// 表示サイズの設定
//=============================================================================
void SetPolygonSize(float fScaleX, float fScaleY)
{
	g_sizPolygon.x = fScaleX;
	g_sizPolygon.y = fScaleY;
}

//=============================================================================
// 表示角度の設定(単位:度)
//=============================================================================
void SetPolygonAngle(float fAngle)
{
	g_rotPolygon.z = fAngle;
}
void SetPolygonAngle(float3 angle)
{
	g_rotPolygon = angle;
}

//=============================================================================
// 左上テクスチャ座標の設定 (0.0≦fU＜1.0, 0.0≦fV＜1.0)
//=============================================================================
void SetPolygonUV(float fU, float fV)
{
	g_posTexFrame.x = fU;
	g_posTexFrame.y = fV;
}

//=============================================================================
// テクスチャフレームサイズの設定 (0.0＜fWidth≦1.0, 0.0＜fHeight≦1.0)
//=============================================================================
void SetPolygonFrameSize(float fWidth, float fHeight)
{
	g_sizTexFrame.x = fWidth;
	g_sizTexFrame.y = fHeight;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetPolygonColor(float fRed, float fGreen, float fBlue)
{
	if (fRed != g_colPolygon.x || fGreen != g_colPolygon.y || fBlue != g_colPolygon.z) {
		g_colPolygon.x = fRed;
		g_colPolygon.y = fGreen;
		g_colPolygon.z = fBlue;
		g_bInvalidate = true;
	}
}

//=============================================================================
// 不透明度の設定
//=============================================================================
void SetPolygonAlpha(float fAlpha)
{
	if (fAlpha != g_colPolygon.w) {
		g_colPolygon.w = fAlpha;
		g_bInvalidate = true;
	}
}