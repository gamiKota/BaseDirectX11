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
#define INPUT_MAX 256


//**********************************************************************************
// グローバル宣言
//**********************************************************************************
static bool	g_keyInput[INPUT_MAX];			// 入力情報
static bool	g_keyOldInput[INPUT_MAX];		// 1フレーム前の入力情報
static int	g_keyRepeatCount[INPUT_MAX];	// リピート用のカウント
BYTE g_force[INPUT_MAX];

// マウス
static HWND		g_mouseHWnd;
static HHOOK	g_mouseHook;
static int		g_wheelCount;
static int		g_wheelValue;
static bool		g_doubleClickMsg[3];
static bool		g_doubleClick[3];
static POINT	g_mousePos;


/**
 * @brief マウスメッセージフック
 */
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
	}

	MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	switch (pMsg->message)
	{
		// ホイール
	case WM_MOUSEWHEEL:
		g_wheelCount += GET_WHEEL_DELTA_WPARAM(pMsg->wParam) / WHEEL_DELTA;
		break;
		// 左ダブルクリック
	case WM_LBUTTONDBLCLK:
		g_doubleClickMsg[0] = true;
		break;
		// 中ダブルクリック
	case WM_MBUTTONDBLCLK:
		g_doubleClickMsg[1] = true;
		break;
		// 右ダブルクリック
	case WM_RBUTTONDBLCLK:
		g_doubleClickMsg[2] = true;
		break;
	}
	return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}



/*
関　数：初期化処理
戻り値：Success codes
*/
void Input::Init() {

	ZeroMemory(g_keyInput, sizeof(g_keyInput));
	ZeroMemory(g_keyOldInput, sizeof(g_keyOldInput));
	ZeroMemory(g_keyRepeatCount, sizeof(g_keyRepeatCount));
	memset(g_force, 255, INPUT_MAX);

	// マウス
	g_mouseHWnd = GetActiveWindow();
	g_mouseHook = SetWindowsHookEx(WH_GETMESSAGE, MouseProc, 0, GetCurrentThreadId());
}


/*
関　数：終了処理
*/
void Input::Uninit() {
	// 特に処理なし
	UnhookWindowsHookEx(g_mouseHook);
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

		if (g_force[i] != 255)
		{
			g_keyInput[i] = g_force[i];
			g_force[i] = 255;
		}

		if (Input::isPress(i)) {
			if (g_keyRepeatCount[i] <= WAIT_INPUT_REPEAT) {
				g_keyRepeatCount[i]++;
			}
		}
		else {
			g_keyRepeatCount[i] = 0;
		}
	}

	//--- マウス
	// マウス座標
	GetCursorPos(&g_mousePos);
	ScreenToClient(g_mouseHWnd, &g_mousePos);
	// マウスホイール
	g_wheelValue = g_wheelCount;
	g_wheelCount = 0;
	// マウスクリック
	for (int i = 0; i < 3; ++i)
	{
		g_doubleClick[i] = g_doubleClickMsg[i];
		g_doubleClickMsg[i] = false;
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


void Input::InstantForce(int nKey, bool bPress) {
	g_force[nKey] = bPress ? 0x80 : 0;
}

bool Input::IsMouseDoubleClick(int nKey) {
	switch (nKey)
	{
	default: return false;
	case VK_LBUTTON: return g_doubleClick[0];
	case VK_MBUTTON: return g_doubleClick[1];
	case VK_RBUTTON: return g_doubleClick[2];
	}
}


const POINT& Input::GetMousePos() {
	return g_mousePos;
}


int Input::GetMouseWheel() {
	return g_wheelValue;
}


// EOF