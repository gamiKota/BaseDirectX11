/**
 * @file Light.cpp
 */


/**
 * @include
 */
#include "Light.h"
#include "D3DClass.h"
#include "ShaderManager.h"
#include "input.h"
#include "Transform.h"
#include "Graphics.h"
#include "Texture.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "imgui.h"
#include "System.h"


/**
 * @constant
 */
#define LIGHT0_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_AMBIENT	XMFLOAT4(0.5f,0.5f,0.5f,1.0f)
#define LIGHT0_SPECULAR	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)

const float3 LIGHT_DIR = float3(1.f, -1.f, 1.f);
const UINT TEX_SIZE = 10000;


Light* Light::m_pLight = nullptr;


Light::Light() :	m_diffuse(XMFLOAT4(0.f, 1.f, 0.f, 1.f)),
					m_ambient(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_specular(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_direction(float3()),
					m_step(0)
{
}

void Light::Uninit() {
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pRTTex);
}


void Light::Awake() {
	UINT viewW = TEX_SIZE;
	UINT viewH = TEX_SIZE;
	// �������猩���i�F��ۑ����Ă������߂̗̈���쐬
	CreateRenderTexture(viewW, viewH, DXGI_FORMAT_R32_FLOAT, &m_pRTTex, &m_pRTView);
	CreateDepthStencil(viewW, viewH, DXGI_FORMAT_D24_UNORM_S8_UINT, &m_pDSView);

	m_direction = float3::Normalize(LIGHT_DIR);
	m_diffuse	= LIGHT0_DIFFUSE;
	m_ambient	= LIGHT0_AMBIENT;
	m_specular	= LIGHT0_SPECULAR;


}


void Light::Start() {

}


void Light::Update() {
	//// ���C�g�̍X�V
	//static float rad = 0.f;
	//
	//if (Input::isPress(VK_LEFT)) {
	//	rad -= (DirectX::XM_PI * 0.5f) / 60.f;
	//}
	//if (Input::isPress(VK_RIGHT)) {
	//	rad += (DirectX::XM_PI * 0.5f) / 60.f;
	//}
	//
	//m_direction.x = sinf(rad) * 50.f;
	//m_direction.z = -cosf(rad) * 50.f;
	//m_direction.y = 30.f;
	//DirectX::XMVECTOR vLightDir;
	//vLightDir = DirectX::XMLoadFloat3(&m_direction);
	//vLightDir = DirectX::XMVectorScale(vLightDir, -1);
	//DirectX::XMStoreFloat3(&m_direction, DirectX::XMVector3Normalize(vLightDir));
	m_transform->LookAt(float3(0.f, -3000.f, 0.f));	// �����͒n��
}


void Light::LastUpdate() {
	if (this == m_pLight) {
		// �V�F�[�_�ɓn���f�[�^��ݒ�
		SHADER_LIGHT buf;
		buf.vLightDir	= XMLoadFloat4(&XMFLOAT4(m_direction.x, m_direction.y, m_direction.z, 0.f));
		buf.vLd			= XMLoadFloat4(&m_pLight->m_diffuse);
		buf.vLa			= XMLoadFloat4(&m_pLight->m_ambient);
		buf.vLs			= XMLoadFloat4(&m_pLight->m_specular);
		ShaderManager::GetInstance().UpdateBuffer("MainLight", &buf);
	}
}


void Light::Shadow() {
	UINT viewW = TEX_SIZE;
	UINT viewH = TEX_SIZE;
	float viewD = 50000.0f;	// �t�@�[�N���b�v����

	// �������猩����i�F��\�����邽�߂̃J�����쐬
	DirectX::XMVECTOR vLPos = DirectX::XMLoadFloat4(&XMFLOAT4(m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z, 0.f));
	DirectX::XMVECTOR vLDir = DirectX::XMLoadFloat4(&XMFLOAT4(m_transform->m_forward.x, m_transform->m_forward.y, m_transform->m_forward.z, 0.f));
	DirectX::XMVECTOR eye = vLPos;
	DirectX::XMVECTOR focus = DirectX::XMVectorAdd(vLPos, vLDir);
	DirectX::XMMATRIX vView = DirectX::XMMatrixLookAtLH(eye, focus, DirectX::XMVectorSet(0, 1, 0, 0));
	DirectX::XMMATRIX vProj = DirectX::XMMatrixOrthographicLH((float)viewW, (float)viewH, 0.1f, viewD);
	DirectX::XMMATRIX vScreen = DirectX::XMMatrixScaling(0.5f, -0.5f, 1.0f) * DirectX::XMMatrixTranslation(0.5f, 0.5f, 0.0f);

	// �`����ύX
	// �e�𔽉f���������I�u�W�F�N�g�̕`��
	float color[] = { 1, 1, 1, 0 };
	D3DClass::GetInstance().SetRenderTarget(viewW, viewH, &m_pRTView, 1, m_pDSView, color);
	{
		// �����ɏ������ލۂ̃V�F�[�_��ݒ�
		// �V�F�[�_��ݒ�
		ShaderManager::GetInstance().BindVS(VS_DEPTHWRITE);
		ShaderManager::GetInstance().BindPS(PS_DEPTHWRITE);
		D3DClass::GetInstance().GetDeviceContext()->HSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->DSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->GSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->CSSetShader(NULL, NULL, 0);
	
		// ���f���f�[�^�`��(�e��)
		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&GameObject::Find("Player")->m_transform->GetMatrix()));
		ShaderManager::GetInstance().UpdateBuffer("MainWorld", &world);
		ModelManager::GetInstance().Draw(E_MODEL::E_MODEL_PLAYER);
	}
	// ���̕`���ɖ߂�
	D3DClass::GetInstance().SetRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, 1, nullptr);

	// �e�N�X�`��
	ShaderManager::GetInstance().SetTexturePS(m_pRTTex, 7);
	// ���C�g�X�N���[��
	SHADER_LIGHT_SCREEN lightS;
	lightS.view = DirectX::XMMatrixTranspose(vView);
	lightS.proj = DirectX::XMMatrixTranspose(vProj);
	lightS.vVPS = DirectX::XMMatrixTranspose(vView * vProj * vScreen);
	ShaderManager::GetInstance().UpdateBuffer("MainLightScreen", &lightS);
}


void Light::SetImGuiVal() {
	ImGui::Image(m_pRTTex, { 300.f, 300.f });
}


// EOF