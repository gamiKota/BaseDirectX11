#ifndef ___HEADER_INPUT_H___
#define ___HEADER_INPUT_H___


//**********************************************************************************
// インクルード部
//**********************************************************************************
#include <Windows.h>


namespace Input
{
	void Init();
	void Uninit();
	void Update();
	/*
	void DrawInput();
	入力で描画するものはない...
	が、デバッグ表示や格ゲーの入力確認など
	ゲームによって用途が変わる
	*/

	/*
	関　数：キープレス
	引　数：仮想キーコード
	戻り値：true  = 引数の仮想キーコードが押されていれば
	false = 引数の仮想キーコードが押されていない
	*/
	bool isPress(int nVertKey);


	/*
	関　数：キートリガー(押したとき
	*/
	bool isTrigger(int nVertKey);


	/*
	関　数：キーリリース(離したとき
	*/
	bool isRelease(int nVertKey);


	/*
	関　数：キーリピート(押し続けたとき
	*/
	bool isRepeat(int nVertKey);


	void InstantForce(int nKey, bool bPress);

	bool IsMouseDoubleClick(int nKey);
	const POINT& GetMousePos();
	int GetMouseWheel();
};



#endif // !___HEADER_INPUT_H___
// EOF