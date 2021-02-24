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
#include "System.h"


/**
 * @constant
 */
#define LIGHT0_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_AMBIENT	XMFLOAT4(0.5f,0.5f,0.5f,1.0f)
#define LIGHT0_SPECULAR	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define LIGHT0_DIR_X	(0.0f)
#define LIGHT0_DIR_Y	(-1.0f)
#define LIGHT0_DIR_Z	(0.0f)

const float SCREEN_NUM = 10;	//!< 影の解像度


Light* Light::m_pLight = nullptr;


Light::Light() :	m_diffuse(XMFLOAT4(0.f, 1.f, 0.f, 1.f)),
					m_ambient(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_specular(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_direction(XMFLOAT3(0.f, 0.f, 1.f)),
					m_step(0)
{
}

void Light::Uninit() {
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pRTTex);
}


void Light::Awake() {
	UINT viewW = SCREEN_WIDTH * SCREEN_NUM;
	UINT viewH = SCREEN_HEIGHT * SCREEN_NUM;
	// 光源から見た景色を保存しておくための領域を作成
	CreateRenderTexture(viewW, viewH, DXGI_FORMAT_R32_FLOAT, &m_pRTTex, &m_pRTView);
	CreateDepthStencil(viewW, viewH, DXGI_FORMAT_D24_UNORM_S8_UINT, &m_pDSView);

	m_direction = XMFLOAT3(LIGHT0_DIR_X, LIGHT0_DIR_Y, LIGHT0_DIR_Z);
	m_diffuse	= LIGHT0_DIFFUSE;
	m_ambient	= LIGHT0_AMBIENT;
	m_specular	= LIGHT0_SPECULAR;
}


void Light::Start() {

}


void Light::Update() {
	//// ライトの更新
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
}


void Light::LastUpdate() {
	UINT viewW = SCREEN_WIDTH * SCREEN_NUM;
	UINT viewH = SCREEN_HEIGHT * SCREEN_NUM;
	float viewD = 10000.0f;

	// 光源から見える景色を表示するためのカメラ作成
	DirectX::XMVECTOR vLPos = DirectX::XMLoadFloat4(&XMFLOAT4(m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z, 0.f));
	DirectX::XMVECTOR vLDir = DirectX::XMLoadFloat4(&XMFLOAT4(m_direction.x, m_direction.y, m_direction.z, 0.f));
	DirectX::XMVECTOR eye = vLPos;
	//DirectX::XMVECTOR focus = DirectX::XMVectorAdd(vLPos, vLDir);
	DirectX::XMVECTOR focus = DirectX::XMVectorAdd(vLPos, DirectX::XMVectorScale(vLDir, 10000.f));
	DirectX::XMMATRIX vView = DirectX::XMMatrixLookAtLH(eye, focus, DirectX::XMVectorSet(0, 1, 0, 0));
	DirectX::XMMATRIX vProj = DirectX::XMMatrixOrthographicLH(10, 10, 0.1f, viewD);
	DirectX::XMMATRIX vScreen =	DirectX::XMMatrixScaling(0.5f, -0.5f, 1.0f) * DirectX::XMMatrixTranslation(0.5f, 0.5f, 0.0f);
	
	// 描画先を変更
	// 影を反映させたいオブジェクトの描画
	float color[] = { 1, 1, 1, 0 };
	D3DClass::GetInstance().SetRenderTarget(viewW, viewH, m_pRTView, m_pDSView, color);
	{
		// 光源に書き込む際のシェーダを設定
		// シェーダを設定
		ShaderManager::GetInstance().BindVS(VS_DEPTHWRITE);
		ShaderManager::GetInstance().BindPS(PS_DEPTHWRITE);
		D3DClass::GetInstance().GetDeviceContext()->HSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->DSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->GSSetShader(NULL, NULL, 0);
		D3DClass::GetInstance().GetDeviceContext()->CSSetShader(NULL, NULL, 0);
	
		// モデルデータ描画(影つけ)
		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&GameObject::Find("Player")->m_transform->GetMatrix()));
		ShaderManager::GetInstance().UpdateBuffer("MainWorld", &world);
		ModelManager::GetInstance().Draw(E_MODEL::E_MODEL_PLAYER);
	}
	if (this == m_pLight) {
		// シェーダに渡すデータを設定
		SHADER_LIGHT buf;
		buf.vLightDir	= XMLoadFloat4(&XMFLOAT4(m_direction.x, m_direction.y, m_direction.z, 0.f));
		buf.vLd			= XMLoadFloat4(&m_pLight->m_diffuse);
		buf.vLa			= XMLoadFloat4(&m_pLight->m_ambient);
		buf.vLs			= XMLoadFloat4(&m_pLight->m_specular);
		buf.view		= DirectX::XMMatrixTranspose(vView);
		buf.proj		= DirectX::XMMatrixTranspose(vProj);
		buf.vVPS		= DirectX::XMMatrixTranspose(vView * vProj * vScreen);
		ShaderManager::GetInstance().UpdateBuffer("MainLight", &buf);
		// テクスチャ
		ShaderManager::GetInstance().SetTexturePS(m_pRTTex, 8);
	}

	// 元の描画先に戻す
	D3DClass::GetInstance().SetRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr);
}


// EOF