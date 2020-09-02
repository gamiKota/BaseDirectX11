#pragma once


//********************************************************************************
// インクルード部
//********************************************************************************
#include <windows.h>
#include <tchar.h>
#include "Singleton.h"
#include <crtdbg.h>
#ifdef _DEBUG
#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   calloc(c, s)          _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   realloc(p, s)         _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _recalloc(p, c, s)    _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _expand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
#define WIN32_LEAN_AND_MEAN

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif


/**
 * @class	System
 * @brief	ゲームシステム管理
 */
class System : public Singleton<System> {
public:
	bool Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
	void Shutdown();
	void Run();

	HINSTANCE GetWinInstance();
	HWND GetWnd();

private:
	bool InitializeWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);

private:
	HINSTANCE	m_hinstance;
	HWND		m_hwnd;
};


// EOF