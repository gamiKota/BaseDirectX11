//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "D3DClass.h"		// �N���X�̐錾��
#include "Graphics.h"		// �E�B���h�E�T�C�Y�̎擾
#include "polygon.h"		// �|���S��
#include "mesh.h"			// ���b�V��
#include "SceneManager.h"	// �V�[���̊Ǘ�
#include "ShaderManager.h"	// �V�F�[�_�̊Ǘ�
#include "System.h"			// �|�C���^�[�̉��


//=============================================================================
// ����������
//=============================================================================
HRESULT D3DClass::Initialize(HWND hWnd, BOOL bWindow) {
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
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

	// �o�b�N�o�b�t�@����
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[2]);
	m_pDeviceContext->RSSetState(m_pRs[2]);

	// �u�����h �X�e�[�g����
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
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	// �[�x�X�e���V���X�e�[�g����
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);

	// �|���S���\��������
	hr = InitPolygon(m_pDevice);
	if (FAILED(hr))
		return hr;

	// ���b�V���\��������
	hr = InitMesh();
	if (FAILED(hr))
		return hr;

	// �V�F�[�_�̏�����
	ShaderManager::GetInstance().Initialize();

	// �V�[���̏�����
	SceneManager::GetInstance().Init();

	return hr;
}


//=============================================================================
// �I������
//=============================================================================
void D3DClass::Uninit(void) {

	// �V�[���̏I������
	SceneManager::GetInstance().Uninit();
	SceneManager::GetInstance().m_scene->Shutdown();

	// �V�F�[�_�̏I��
	ShaderManager::GetInstance().Terminate();

	// ���b�V���\���I������
	UninitMesh();

	// �|���S���\���I������
	UninitPolygon();

	// �[�x�X�e���V���X�e�[�g���
	for (int i = 0; i < _countof(m_pDSS); ++i) {
		SAFE_RELEASE(m_pDSS[i]);
	}

	// �u�����h �X�e�[�g���
	for (int i = 0; i < MAX_BLENDSTATE; ++i) {
		SAFE_RELEASE(m_pBlendState[i]);
	}

	// ���X�^���C�U �X�e�[�g���
	for (int i = 0; i < MAX_CULLMODE; ++i) {
		SAFE_RELEASE(m_pRs[i]);
	}

	// �o�b�N�o�b�t�@���
	ReleaseBackBuffer();

	// �X���b�v�`�F�[�����
	SAFE_RELEASE(m_pSwapChain);

	// �f�o�C�X �R���e�L�X�g�̊J��
	SAFE_RELEASE(m_pDeviceContext);

	// �f�o�C�X�̊J��
	SAFE_RELEASE(m_pDevice);
}


//=============================================================================
// �X�V����
//=============================================================================
void D3DClass::Update(void) {
	// �|���S���\���X�V
	UpdatePolygon();

	// �V�[���̍X�V����
	SceneManager::GetInstance().Update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void D3DClass::Draw(void) {
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �V�[���̕`�揈��
	SceneManager::GetInstance().Draw();
}


//=============================================================================
// �f�o�C�X�擾
//=============================================================================
ID3D11Device* D3DClass::GetDevice()
{
	return m_pDevice;
}

//=============================================================================
// �f�o�C�X �R���e�L�X�g�擾
//=============================================================================
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_pDeviceContext;
}

void D3DClass::SwitchingBuffer()
{
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pSwapChain->Present(m_uSyncInterval, 0);
}

//=============================================================================
// �[�x�o�b�t�@�L����������
//=============================================================================
void D3DClass::SetZBuffer(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[1], 0);
}

//=============================================================================
// �[�x�o�b�t�@�X�V�L����������
//=============================================================================
void D3DClass::SetZWrite(bool bEnable)
{
	m_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : m_pDSS[0], 0);
}

//=============================================================================
// �u�����h�X�e�[�g�ݒ�
//=============================================================================
void D3DClass::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState], blendFactor, 0xffffffff);
	}
}


//=============================================================================
// �J�����O�ݒ�
//=============================================================================
void D3DClass::SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		m_pDeviceContext->RSSetState(m_pRs[nCullMode]);
	}
}


//=============================================================================
// �o�b�N�o�b�t�@����
//=============================================================================
HRESULT D3DClass::CreateBackBuffer(void) {
	// �����_�[�^�[�Q�b�g�r���[����
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	// Z�o�b�t�@�p�e�N�X�`������
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

	// Z�o�b�t�@�^�[�Q�b�g�r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// �r���[�|�[�g�ݒ�
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
// �o�b�N�o�b�t�@���
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