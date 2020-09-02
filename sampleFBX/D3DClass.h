#pragma once


//*****************************************************************************
// リンカー
//*****************************************************************************
#pragma comment(lib, "d3d11")


//*****************************************************************************
// インクルード部
//*****************************************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include "Singleton.h"


// 本来はヘッダに書かない方が良い
using namespace DirectX;


//*****************************************************************************
// マクロ・定数定義
//*****************************************************************************
#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数


enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// カリングしない
	CULLMODE_CW,							// 前面カリング
	CULLMODE_CCW,							// 背面カリング

	MAX_CULLMODE
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

// 頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT3 nor;		// 法線ベクトル
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_3D;


/**
 * @class	DirectX関連の処理
 * @brief	DirectXに依存する処理をまとめた関数
 */
class D3DClass : public Singleton<D3DClass>
{
public:

	/**
	 * @brief	初期化
	 * @details	オブジェクトの初期化
	 */
	HRESULT Initialize(HWND hWnd, BOOL bWindow);

	/**
	 * @brief	終了処理
	 * @details	オブジェクトの終了処理
	 */
	void Uninit();

	/**
	 * @brief	更新処理
	 * @details	オブジェクトの更新処理
	 */
	void Update();

	/**
	 * @brief	描画処理
	 * @details	オブジェクトの描画
	 */
	void Draw();

	/**
	 * @brief	デバイスの取得
	 */
	ID3D11Device* GetDevice();

	/**
	 * @brief	デバイス コンテキスト取得
	 */
	ID3D11DeviceContext* GetDeviceContext();

	/**
	 * @brief	バックバッファとフロントバッファの入れ替え
	 */
	void SwitchingBuffer();

	/**
	 * @brief	深度バッファ有効無効制御
	 * @param	true = 深度バッファ有効
	 */
	void SetZBuffer(bool bEnable);

	/**
	 * @brief	深度バッファ更新有効無効制御
	 * @param	true = 深度バッファ更新有効
	 */
	void SetZWrite(bool bEnable);

	
	void SetBlendState(int nBlendState);

	
	void SetCullMode(int nCullMode);

private:
	ID3D11Device*				m_pDevice;						//!< デバイス
	ID3D11DeviceContext*		m_pDeviceContext;				//!< デバイス コンテキスト
	IDXGISwapChain*				m_pSwapChain;					//!< スワップチェーン
	ID3D11RenderTargetView*		m_pRenderTargetView;			//!< フレームバッファ
	ID3D11Texture2D*			m_pDepthStencilTexture;			//!< Zバッファ用メモリ
	ID3D11DepthStencilView*		m_pDepthStencilView;			//!< Zバッファ
	UINT						m_uSyncInterval;				//!< 垂直同期 (0:無, 1:有)
	ID3D11RasterizerState*		m_pRs[MAX_CULLMODE];			//!< ラスタライザ ステート
	ID3D11BlendState*			m_pBlendState[MAX_BLENDSTATE];	//!< ブレンド ステート
	ID3D11DepthStencilState*	m_pDSS[2];						//!< Zバッファ更新しない

	/**
	 * @brief	バックバッファの生成
	 */
	HRESULT CreateBackBuffer(void);

	/**
	 * @brief	バックバッファ解放
	 */
	void ReleaseBackBuffer();
};

// EOF