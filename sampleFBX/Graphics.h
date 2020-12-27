/**
 * @file	Graphics.h
 * @brief	3Dグラフィックスの描画一括管理
 *
 * @author	Kota Nakagami
 * @data	2019/12/06(金)
 *
 * @version	1.00
 */
#pragma once

 //********************************************************************************
 // インクルード部
 //********************************************************************************
#include <Windows.h>
#include "Singleton.h"


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
#define		SCREEN_WIDTH	(1280)							// ウインドウの幅
#define		SCREEN_HEIGHT	(720)							// ウインドウの高さ
#define		SCREEN_CENTER_X	(SCREEN_WIDTH/2)				// ウインドウの中心Ｘ座標
#define		SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)				// ウインドウの中心Ｙ座標
#define		SCREEN_RATIO	(SCREEN_WIDTH / SCREEN_HEIGHT)	// 画面比


/**
 * @enum	class Library
 * @brief	3Dグラフィックス描画機能の種類
 *			マクロで機能を分けた方が良さげ
 */
enum class Library
{
	DirectX,	// DirectX11
	OpenGL,		// OpenGL
};


/**
 * @class	Graphics
 * @brief	描画管理
 */
class Graphics : public Singleton<Graphics> {
public:
	/**
	 * @brief ライブラリとゲームの初期化
	 */
	bool Init(Library, int, int, HWND);

	/**
	 * @brief ゲームの更新
	 */
	void Update();

	/**
	 * @brief ゲームの描画
	 */
	void Draw();

	/**
	 * @brief ライブラリとゲームの終了処理
	 */
	void Shutdown();

	/**
	 * @brief	FPSカウント設定
	 * @param	FPSCount
	 */
	void SetFPSCount(int FPSCount);

private:
	Library m_library;		//!< ライブラリ
	int		m_nCountFPS;	//!< FPSカウンタ
};


// EOF