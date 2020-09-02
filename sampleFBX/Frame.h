#pragma once


//********************************************************************************
// インクルード部
//********************************************************************************
#include <windows.h>
#include <tchar.h>
#include "Singleton.h"


/**
 * @class	Frame
 * @brief	フレーム管理
 */
class Frame : public Singleton<Frame>
{
public:

	/**
	 * @brief	初期化
	 */
	void Init();

	/**
	 * @brief	終了処理
	 */
	void Uninit();

	/**
	 * @brief	更新処理
	 */
	void Update();

	/**
	 * @brief	ゲーム更新許可
	 * @return	true = ゲーム更新
	 */
	bool isUpdateGame();

	/**
	 * @brief	フレームの取得
	 * @return	DWORD FPSカウント
	 */
	DWORD GetFrame();

	/**
	 * @brief	ゲーム開始からの経過時間の取得
	 * @return	DWORD 経過時間
	 */
	DWORD GetPassageTime();

	/**
	 * @brief	FPSカウントの描画
	 */
	void DrawFPS();

private:
	DWORD	m_startTime;	//!< アプリケーション起動時の時間
	DWORD	m_oldTime;		//!< 前の時間
	DWORD	m_nowTime;		//!< 今の時間
	DWORD	m_Frame;		//!< フレーム数
};


// EOF