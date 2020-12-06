//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "Graphics.h"		// �錾�N���X��
#include "input.h"			// ����
#include "Light.h"			// �������
#include "debugproc.h"		// �f�o�b�O�\���p
#include "D3DClass.h"		// D3D�֘A
#include "Frame.h"			// �t���[���Ǘ�
#include "polygon.h"		// �f�o�b�O�����F�̕ύX
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "System.h"


//********************************************************************************
// �}�N���E�萔��`
//********************************************************************************
const bool	FULL_SCREEN = false;		// �t���X�N���[�����[�h


bool Graphics::Init(Library eLibrary, int screenWidth, int screenHeight, HWND hwnd) {

	HRESULT hr = S_OK;

	m_nCountFPS = 0;
	m_library = eLibrary;

	// ���͏���������
	Input::Init();

	// �����̏�����
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

	// �f�o�b�O������\��������
	hr = InitDebugProc();

	return hr;	// �������̐���
}


void Graphics::Shutdown() {

	// �f�o�b�O������\���I������
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

	// �����I������
	UninitLight();

	// ���͏����I������
	Input::Uninit();

	return;
}


void Graphics::Update() {

	// ���͏����X�V
	Input::Update();	// �K��Update�֐��̐擪�Ŏ��s.

	// �f�o�b�O������\���X�V
	UpdateDebugProc();

	// �f�o�b�O������ݒ�
	StartDebugProc();

	// �t���[���\��
	Frame::GetInstance().DrawFPS();

	// �����X�V
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

	// �f�o�b�O������\��
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	DrawDebugProc();


	switch (m_library) {
	case Library::DirectX:	D3DClass::GetInstance().SwitchingBuffer();	break;
	case Library::OpenGL:	break;
	}
}


/**
 * @brief	FPS�J�E���g�ݒ�
 * @param	FPSCount
 */
void Graphics::SetFPSCount(int FPSCount) {
	m_nCountFPS = FPSCount;
}


// EOF