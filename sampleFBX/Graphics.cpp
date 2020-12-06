//********************************************************************************
// インクルード部
//********************************************************************************
#include "Graphics.h"		// 宣言クラス先
#include "input.h"			// 入力
#include "Light.h"			// 光源情報
#include "debugproc.h"		// デバッグ表示用
#include "D3DClass.h"		// D3D関連
#include "Frame.h"			// フレーム管理
#include "polygon.h"		// デバッグ文字色の変更
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "System.h"


//********************************************************************************
// マクロ・定数定義
//********************************************************************************
const bool	FULL_SCREEN = false;		// フルスクリーンモード


bool Graphics::Init(Library eLibrary, int screenWidth, int screenHeight, HWND hwnd) {

	HRESULT hr = S_OK;

	m_nCountFPS = 0;
	m_library = eLibrary;

	// 入力処理初期化
	Input::Init();

	// 光源の初期化
	InitLight();

	// Initialize the graphics scene.
	switch (m_library) {
		// Create the Direct3D object.
		// Initialize the Direct3D object.
	case Library::DirectX:
		hr = D3DClass::GetInstance().GetInstance().Initialize(hwnd, !FULL_SCREEN);
		break;

		// Create the OpenGL object.
		// Initialize the OpenGL object.
	case Library::OpenGL:
		break;
	}

//#if _DEBUG
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	ImGui::StyleColorsLight();
//
//	ImGui_ImplWin32_Init(hwnd);
//	ImGui_ImplDX11_Init(D3DClass::GetInstance().GetDevice(), D3DClass::GetInstance().GetDeviceContext());
//#endif

	// デバッグ文字列表示初期化
	hr = InitDebugProc();

	return hr;	// 初期化の成功
}


void Graphics::Shutdown() {

	// デバッグ文字列表示終了処理
	UninitDebugProc();

	// Uninit the graphics scene.
	switch (m_library) {
		// Uninit the Direct3D object.
	case Library::DirectX:
//#if _DEBUG
//		ImGui_ImplDX11_Shutdown();
//		ImGui_ImplWin32_Shutdown();
//		ImGui::DestroyContext();
//#endif
		D3DClass::GetInstance().Uninit();
		break;

		// Uninit the OpenGL object.
	case Library::OpenGL:
		break;
	}

	// 光源終了処理
	UninitLight();

	// 入力処理終了処理
	Input::Uninit();

	return;
}


void Graphics::Update() {

	// 入力処理更新
	Input::Update();	// 必ずUpdate関数の先頭で実行.

	// デバッグ文字列表示更新
	UpdateDebugProc();

	// デバッグ文字列設定
	StartDebugProc();

	// フレーム表示
	Frame::GetInstance().DrawFPS();

	// 光源更新
	UpdateLight();

	// Update the graphics scene.
	switch (m_library) {
		// Update the Direct3D object.
	case Library::DirectX:
		D3DClass::GetInstance().Update();
		break;

		// Uninit the OpenGL object.
	case Library::OpenGL:	break;
	}

//#if _DEBUG
//	ImGui_ImplDX11_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//#endif
}


void Graphics::Draw() {
	// Render the graphics scene.
	switch (m_library) {
		// Render the Direct3D object.
	case Library::DirectX:
		D3DClass::GetInstance().Draw();
		//ImGui::Render();
		//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		break;

		// Render the OpenGL object.
	case Library::OpenGL:	break;
	}

#if _DEBUG
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	// デバッグ文字列表示
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	DrawDebugProc();


	switch (m_library) {
	case Library::DirectX:	D3DClass::GetInstance().SwitchingBuffer();	break;
	case Library::OpenGL:	break;
	}
}


/**
 * @brief	FPSカウント設定
 * @param	FPSCount
 */
void Graphics::SetFPSCount(int FPSCount) {
	m_nCountFPS = FPSCount;
}


// EOF