//********************************************************************************
// インクルード部
//********************************************************************************
#include "Frame.h"		// このクラスの宣言先
#include "debugproc.h"	// デバッグ表示用


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
static const int VAL_FRAME = 60;


void Frame::Init() {
	// フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	m_startTime = m_nowTime = m_oldTime = (DWORD)timeGetTime();
	m_Frame = 0;
}


void Frame::Uninit() {
	// タイマ設定を元に戻す
	timeEndPeriod(1);
}


void Frame::Update() {
	m_nowTime = timeGetTime();	// 現在の時間取得(ミリ秒)
}


bool Frame::isUpdateGame() {
	if ((m_nowTime - m_oldTime) >= 1000 / VAL_FRAME) {
		m_Frame = (1000 / (m_nowTime - m_oldTime));
		m_oldTime = m_nowTime;
		return true;
	}
	return false;
}


DWORD Frame::GetFrame() {
	return m_Frame;
}


DWORD Frame::GetPassageTime() {
	return m_nowTime - m_startTime;
}


void Frame::DrawFPS() {
	PrintDebugProc("FPS:%d\n\n", m_Frame);
}


// EOF