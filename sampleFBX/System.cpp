//********************************************************************************
// インクルード部
//********************************************************************************
#include "Frame.h"		// フレーム管理
#include "Graphics.h"	// 描画
#include "D3DClass.h"	// D3D関係
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "System.h"		// 宣言クラス先


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
#define CLASS_NAME		_T("AppClass")					// ウインドウのクラス名
#define WINDOW_NAME		_T("AT13A222_21_中上皓太")		// ウインドウのキャプション名
static const Library LIBRARY_MODE = Library::DirectX;	// ライブラリモード
static const BOOL CURSOR_MODE = TRUE;


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int	OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);

#if _DEBUG
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

bool System::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	// ウィンドウの初期化
	this->InitializeWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// フレームの初期化
	Frame::GetInstance().Init();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	// Initialize the graphics object.
	if (FAILED(Graphics::GetInstance().Init(Library::DirectX, 0, 0, m_hwnd))) {
		return false;
	}

#if _DEBUG
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(D3DClass::GetInstance().GetDevice(), D3DClass::GetInstance().GetDeviceContext());

	// Our state
	//clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif

	return true;
}


void System::Shutdown() {
	// フレーム機能の終了処理
	Frame::GetInstance().Uninit();

#if _DEBUG
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

	// Release the graphics object.
	Graphics::GetInstance().Shutdown();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, m_hinstance);

	// COM終了処理
	CoUninitialize();

	return;
}


void System::Run() {
	MSG msg;

	// メッセージループ
	for (;;) {
		// ゲーム終了
		if (0/* 何かゲームの終了条件 */) {
			break;
		}

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			Frame::GetInstance().Update();

			if (Frame::GetInstance().isUpdateGame()) {	

#if _DEBUG
				// Start the Dear ImGui frame
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("[inspector]");
				
				ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
#endif

				Graphics::GetInstance().Update();	// 更新処理

#if _DEBUG
				ImGui::End();
#endif

				Graphics::GetInstance().GetInstance().Draw();	// 描画処理
			}
		}
	}
}


bool System::InitializeWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		nullptr,
		LoadCursor(nullptr, IDC_WAIT),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		nullptr
	};
	ShowCursor(CURSOR_MODE);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBoxW(NULL, L"COMの初期化に失敗しました。", L"error", MB_OK);
		return false;
	}

	// インスタンス ハンドル保存
	m_hinstance = hInstance;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域サイズからウィンドウ サイズ算出
	DWORD dwStyle = 0;
	DWORD dwExStyle = 0;
	RECT  rc;
	dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	// ウィンドウの作成
	m_hwnd = CreateWindowEx(dwExStyle,
		CLASS_NAME,
		WINDOW_NAME,
		dwStyle,
		1,		// ウィンドウの左座標
		1,		// ウィンドウの上座標
		rc.right - rc.left,	// ウィンドウ横幅
		rc.bottom - rc.top,	// ウィンドウ縦幅
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	return true;
}


HINSTANCE System::GetWinInstance() {
	return m_hinstance;
}


HWND System::GetWnd() {
	return m_hwnd;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
#if _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return S_OK;
#endif
	switch (uMsg) {
	case WM_CREATE:										//----- ウィンドウが生成された

		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:									//----- ウィンドウ破棄指示がきた
		PostQuitMessage(0);								// システムにスレッドの終了を要求
		break;
	case WM_KEYDOWN:									//----- キーボードが押された
		switch (wParam) {
		case VK_ESCAPE:									// [ESC]キーが押された
			PostMessage(hWnd, WM_CLOSE, 0, 0);			// [x]が押されたように振舞う
			return 0;
		}
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;							// [Alt]+[Enter]時のBEEPを抑止
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//=============================================================================
// WM_CREATEメッセージハンドラ
//=============================================================================
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// クライアント領域サイズをSCREEN_WIDTH×SCREEN_HEIGHTに再設定.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) {
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	// IME無効化
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1を返すとCreateWindow[Ex]が失敗する.
}


// EOF