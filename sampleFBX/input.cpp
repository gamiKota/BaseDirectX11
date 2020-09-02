//**********************************************************************************
// インクルード部
//**********************************************************************************
#include "input.h"
#include "Frame.h"
#include "System.h"


//**********************************************************************************
// マクロ・定数定義
//**********************************************************************************
#define			WAIT_INPUT_REPEAT ((float)Frame::GetInstance().GetFrame() / 3)
static const int			KEYS_MASK = 0x8000;
static const unsigned char INPUT_MAX = 255;


//**********************************************************************************
// グローバル宣言
//**********************************************************************************
static bool	g_keyInput[INPUT_MAX];			// 入力情報
static bool	g_keyOldInput[INPUT_MAX];		// 1フレーム前の入力情報
static int	g_keyRepeatCount[INPUT_MAX];	// リピート用のカウント


/*
関　数：初期化処理
戻り値：Success codes
*/
void Input::Init() {

	ZeroMemory(g_keyInput, sizeof(g_keyInput));
	ZeroMemory(g_keyOldInput, sizeof(g_keyOldInput));
	ZeroMemory(g_keyRepeatCount, sizeof(g_keyRepeatCount));
}


/*
関　数：終了処理
*/
void Input::Uninit() {
	// 特に処理なし
}


/*
関　数：更新処理
*/
void Input::Update() {

	// 前のフレームの入力情報
	memcpy(g_keyOldInput, g_keyInput, sizeof(g_keyOldInput));

	//キー押下判定
	//押下状態ならtrue,非押下状態ならfalse
	for (int i = 0; i < INPUT_MAX; i++) {
		g_keyInput[i] = (GetAsyncKeyState(i) & KEYS_MASK) ? true : false;
	}

	// リピートのカウント
	for (int i = 0; i < INPUT_MAX; i++) {
		if (Input::isPress(i)) {
			if (g_keyRepeatCount[i] <= WAIT_INPUT_REPEAT) {
				g_keyRepeatCount[i]++;
			}
		}
		else {
			g_keyRepeatCount[i] = 0;
		}
	}
}


/*
関　数：キープレス
引　数：仮想キーコード
戻り値：true  = 引数の仮想キーコードが押されていれば
		false = 引数の仮想キーコードが押されていない
*/
bool Input::isPress(int nVertKey) {
	return g_keyInput[nVertKey];
}


/*
関　数：キートリガー(押したとき
*/
bool Input::isTrigger(int nVertKey) {
	return (!g_keyOldInput[nVertKey] && g_keyInput[nVertKey]) ? true : false;
}


/*
関　数：キーリリース(離したとき
*/
bool Input::isRelease(int nVertKey) {
	return (g_keyOldInput[nVertKey] && !g_keyInput[nVertKey]) ? true : false;
}


/*
関　数：キーリピート(押し続けたとき
*/
bool Input::isRepeat(int nVertKey) {
	if (g_keyRepeatCount[nVertKey] == 1) {						// 最初の一回目
		return true;
	}
	else if (g_keyRepeatCount[nVertKey] > WAIT_INPUT_REPEAT) {	// 押し続けたとき
		return true;
	}
	return false;												// 押されてない or まだ押し続けてる間
}


// EOF