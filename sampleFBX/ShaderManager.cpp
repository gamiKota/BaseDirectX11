#include "ShaderManager.h"
#include "ShaderBuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "System.h"


using namespace DirectX;


ShaderBuffer g_worldBuf;		//!< �萔�o�b�t�@
DirectX::XMFLOAT4X4 g_world;	//!< �萔�o�b�t�@�Ɋi�[�����f�[�^

ShaderBuffer g_ViewProjBuf;			//!< �萔�o�b�t�@
DirectX::XMFLOAT4X4 g_ViewProj[2];	//!< �萔�o�b�t�@�Ɋi�[�����f�[�^


XMFLOAT4 g_value;
ShaderBuffer g_valueBuf;


void ShaderManager::Initialize() {

	HRESULT hr;

	// ���_�V�F�[�_
	m_VS[E_SHADER_VS_FBX] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_FBX]->Create("data/shader/FbxModelVertex.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK);}

	m_VS[E_SHADER_VS_OUTLINE] = new VertexShader(LAYOUT_FBX);
	hr = m_VS[E_SHADER_VS_OUTLINE]->Create("data/shader/OutLineVS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	m_VS[E_SHADER_VS_DEFAULT] = new VertexShader(LAYOUT_PCUN);
	hr = m_VS[E_SHADER_VS_DEFAULT]->Create("data/shader/VertexShader.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to VS.", NULL, MB_OK); }

	// �W�I���g���V�F�[�_
	m_GS[E_SHADER_GS_DEFAULT] = new GeometryShader;
	hr = m_GS[E_SHADER_GS_DEFAULT]->Create("data/shader/GeometryShader.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	m_GS[E_SHADER_GS_LINE] = new GeometryShader;
	hr = m_GS[E_SHADER_GS_LINE]->Create("data/shader/LineGS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to GS.", NULL, MB_OK); }

	// �s�N�Z���V�F�[�_
	m_PS[E_SHADER_PS_FBX] = new PixelShader;
	hr = m_PS[E_SHADER_PS_FBX]->Create("data/shader/FbxModelPixel.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_MONOCHRO] = new PixelShader;
	hr = m_PS[E_SHADER_PS_MONOCHRO]->Create("data/shader/MonochromePS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_PHONG] = new PixelShader;
	hr = m_PS[E_SHADER_PS_PHONG]->Create("data/shader/PhongPS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_TOON] = new PixelShader;
	hr = m_PS[E_SHADER_PS_TOON]->Create("data/shader/ToonPS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	m_PS[E_SHADER_PS_OUTLINE] = new PixelShader;
	hr = m_PS[E_SHADER_PS_OUTLINE]->Create("data/shader/OutLinePS.cso");
	if (FAILED(hr)) { MessageBoxW(0, L"Failed to PS.", NULL, MB_OK); }

	// �萔�o�b�t�@
	// �V�F�[�_�Ƀf�[�^��n���ۂɂ́A
	// �K��16�o�C�g�A���C�����g(float�^4��)�Œ萔�o�b�t�@���쐬���Ȃ��Ƃ����Ȃ�
	g_worldBuf.Create(sizeof(g_world));
	g_ViewProjBuf.Create(sizeof(g_ViewProj));
	g_valueBuf.Create(sizeof(g_value));
}


void ShaderManager::Terminate() {
	for (int i = 0; i < E_SHADER_PS_MAX; ++i) {
		SAFE_DELETE(m_PS[i])
	}
	for (int i = 0; i < E_SHADER_GS_MAX; ++i) {
		SAFE_DELETE(m_GS[i])
	}
	for (int i = 0; i < E_SHADER_VS_MAX; ++i) {
		SAFE_DELETE(m_VS[i])
	}
}


void ShaderManager::Bind(E_SHADER shader, E_SHADER_GS gs) {
	switch (shader)
	{
	case E_SHADER_FBX:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_FBX]->Bind();
		break;

	case E_SHADER_MONOCHROME:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_MONOCHRO]->Bind();
		break;

	case E_SHADER_PHONG:
		m_VS[E_SHADER_VS_DEFAULT]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_PHONG]->Bind();
		break;

	case E_SHADER_TOON:
		m_VS[E_SHADER_VS_FBX]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_TOON]->Bind();
		break;

	case E_SHADER_OUTLINE:
		m_VS[E_SHADER_VS_OUTLINE]->Bind();
		//m_GS[gs]->Bind();
		m_PS[E_SHADER_PS_OUTLINE]->Bind();
		break;

	case E_SHADER_MAX:
		break;
	default:
		break;
	}
}


void ShaderManager::UpdateBuffer(XMFLOAT4X4 world) {

	// ���[���h���W
	g_world = world;
	// ���[���h�ϊ��s������߂�
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(0.f, 0.f, 0.f);

	// �s��̕��я����قȂ邽�߁A�V�F�[�_�ɓn���ۂɓ]�u���s��
	trans = DirectX::XMMatrixTranspose(trans);
	DirectX::XMStoreFloat4x4(&g_world, trans);

	// �r���[�E�v���W�F�N�V�����s��
	g_ViewProj[0] = CCamera::Get()->GetView();
	g_ViewProj[1] = CCamera::Get()->GetProj();

	// �J�������W
	g_value.x = CCamera::Get()->m_transform->m_position.x;
	g_value.y = CCamera::Get()->m_transform->m_position.y;
	g_value.z = CCamera::Get()->m_transform->m_position.z;
	g_value.w = 1.f;

	
	//--- �o�b�t�@�̍X�V
	g_worldBuf.UpdateSource(&g_world);
	g_ViewProjBuf.UpdateSource(&g_ViewProj);
	g_valueBuf.UpdateSource(&g_value);

	//--- �V�F�[�_�ɓn��
	// ���_�V�F�[�_
	g_worldBuf.BindVS(3);
	g_ViewProjBuf.BindVS(4);
	// �s�N�Z���V�F�[�_
	g_valueBuf.BindPS(5);
}


// EOF