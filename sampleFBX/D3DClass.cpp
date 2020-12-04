//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "D3DClass.h"		// クラスの宣言先
#include "Graphics.h"		// ウィンドウサイズの取得
#include "polygon.h"		// ポリゴン
#include "mesh.h"			// メッシュ
#include "SceneManager.h"	// シーンの管理
#include "ShaderManager.h"	// シェーダの管理
#include "System.h"			// ポインターの解放


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT D3DClass::Initialize(HWND hWnd, BOOL bWindow) {
	HRESULT hr = S_OK;

	// デバイス、スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = bWindow;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext);
	if (FAILED(hr)) {
		return hr;
	}

	// バックバッファ生成
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[2]);
	m_pDeviceContext->RSSetState(m_pRs[2]);

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[0]);
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);

	// ポリゴン表示初期化
	hr = InitPolygon(m_pDevice);
	if (FAILED(hr))
		return hr;

	// メッシュ表示初期化
	hr = InitMesh();
	if (FAILED(hr))
		return hr;

	// シェーダの初期化
	ShaderManager::GetInstance().Initialize();

	// シーンの初期化
	SceneManager::GetInstance().Init();

	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================
void D3DClass::Uninit(void) {

	// シーンの終了処理
	SceneManager::GetInstance().Uninit();
	SceneManager::GetInstance().m_scene->Shutdown();

	// シェーダの終了
	ShaderManager::GetInstance().Terminate();

	// メッシュ表示終了処理
	UninitMesh();

	// ポリゴン表示終了処理
	UninitPolygon();

	// 深度ステンシルステート解放
	for (int i = 0; i < _countof(m_pDSS); ++i) {
		SAFE_RELEASE(m_pDSS[i]);
	}

	// ブレンド ステート解放
	for (int i = 0; i < MAX_BLENDSTATE; ++i) {
		SAFE_RELEASE(m_pBlendState[i]);
	}

	// ラスタライザ ステート解放
	for (int i = 0; i < MAX_CULLMODE; ++i) {
		SAFE_RELEASE(m_pRs[i]);
	}

	// バックバッファ解放
	ReleaseBackBuffer();

	// スワップチェーン解放
	SAFE_RELEASE(m_pSwapChain);

	// デバイス コンテキストの開放
	SAFE_RELEASE(m_pDeviceContext);

	// デバイスの開放
	SAFE_RELEASE(m_pDevice);
}


//=============================================================================
// 更新処理
//=============================================================================
void D3DClass::Update(void) {
	// ポリゴン表示更新
	UpdatePolygon();

	// シーンの更新処理
	SceneManager::GetInstance().Update();
}


//=============================================================================
// 描画処理
//=============================================================================
void D3DClass::Draw(void) {
	// バックバッファ＆Ｚバッファのクリア
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// シーンの描画処理
	SceneManager::GetInstance().Draw();
}


//=============================================================================
// デバイス取得
//=============================================================================
ID3D11Device* D3DClass::GetDevice()
{
	return m_pDevice;
}

//=============================================================================
// デバイス コンテキスト取得
//=============================================================================
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_pDeviceContext;
}

void D3DClass::SwitchingBuffer()
{
	// バックバッファとフロントバッファの入れ替え
	m_pSwapChain->Present(m_uSyncInterval, 0);
}

//=============================================================================
// 深度バッファ有効無効制御
//=============================================================================
void D3DClass::SetZBuffer(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[1], 0);
}

//=============================================================================
// 深度バッファ更新有効無効制御
//=============================================================================
void D3DClass::SetZWrite(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[0], 0);
}

//=============================================================================
// ブレンドステート設定
//=============================================================================
void D3DClass::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState], blendFactor, 0xffffffff);
	}
}


//=============================================================================
// カリング設定
//=============================================================================
void D3DClass::SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		m_pDeviceContext->RSSetState(m_pRs[nCullMode]);
	}
}


//=============================================================================
// バックバッファ生成
//=============================================================================
HRESULT D3DClass::CreateBackBuffer(void) {
	// レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	// Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = SCREEN_WIDTH;
	td.Height = SCREEN_HEIGHT;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthStencilTexture);
	if (FAILED(hr)) {
		return hr;
	}

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// 各ターゲットビューをレンダーターゲットに設定
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	return S_OK;
}


//=============================================================================
// バックバッファ解放
//=============================================================================
void D3DClass::ReleaseBackBuffer() {
	if (m_pDeviceContext) {
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilTexture);
	SAFE_RELEASE(m_pRenderTargetView);
}


// EOF