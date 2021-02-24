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


Light* Light::m_pLight = nullptr;


Light::Light() :	m_diffuse(XMFLOAT4(1.f, 1.f, 1.f, 1.f)),
					m_ambient(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_specular(XMFLOAT4(0.f, 0.f, 0.f, 1.f)),
					m_direction(XMFLOAT3(0.f, 0.f, 1.f))
{
}


void Light::Awake() {
	m_direction = XMFLOAT3(LIGHT0_DIR_X, LIGHT0_DIR_Y, LIGHT0_DIR_Z);
	m_diffuse	= LIGHT0_DIFFUSE;
	m_ambient	= LIGHT0_AMBIENT;
	m_specular	= LIGHT0_SPECULAR;
}


void Light::Start() {

}


void Light::Update() {
	// ライトの更新
	static float rad = 0.f;

	if (Input::isPress(VK_LEFT)) {
		rad -= (DirectX::XM_PI * 0.5f) / 60.f;
	}
	if (Input::isPress(VK_RIGHT)) {
		rad += (DirectX::XM_PI * 0.5f) / 60.f;
	}

	m_direction.x = sinf(rad) * 50.f;
	m_direction.z = -cosf(rad) * 50.f;
	m_direction.y = 30.f;
	DirectX::XMVECTOR vLightDir;
	vLightDir = DirectX::XMLoadFloat3(&m_direction);
	vLightDir = DirectX::XMVectorScale(vLightDir, -1);
	DirectX::XMStoreFloat3(&m_direction, DirectX::XMVector3Normalize(vLightDir));
}


void Light::LastUpdate() {
	// 光源から見える景色を表示するためのカメラ作成
	//DirectX::XMVECTOR vLPos = DirectX::XMLoadFloat4(&value.pLight->GetPos());
	//DirectX::XMVECTOR vLDir = DirectX::XMLoadFloat4(&value.pLight->GetDir());
	//DirectX::XMVECTOR eye = vLPos;
	//DirectX::XMVECTOR focus = DirectX::XMVectorAdd(vLPos, DirectX::XMVectorScale(vLDir, viewD));
	//DirectX::XMMATRIX vView = DirectX::XMMatrixLookAtLH(eye, focus, DirectX::XMVectorSet(0, 1, 0, 0));
	//DirectX::XMMATRIX vProj = DirectX::XMMatrixOrthographicLH(10, 10, 0.1f, viewD);
	//DirectX::XMMATRIX vScreen =	DirectX::XMMatrixScaling(0.5f, -0.5f, 1.0f) * DirectX::XMMatrixTranslation(0.5f, 0.5f, 0.0f);

	//// 描画先を変更
	//// 影を反映させたいオブジェクトの描画
	//float color[] = { 1, 1, 1, 0 };
	////D3DClass::GetInstance().SetRenderTarget(viewW, viewH, &m_pRTView, m_pDSView, color);
	//{
	//	// 光源に書き込む際のシェーダを設定
	//	// シェーダを設定
	//	ShaderManager::GetInstance().BindVS(VS_DEPTHWRITE);
	//	ShaderManager::GetInstance().BindPS(PS_DEPTHWRITE);
	//
	//	// シェーダで利用するデータを設定
	//	value.SB_World = DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f);
	//	value.SB_VP->view = DirectX::XMMatrixTranspose(vView);
	//	value.SB_VP->proj = DirectX::XMMatrixTranspose(vProj);
	//
	//	// 設定したデータをシェーダに渡す
	//	value.SB_World.BindVS(0);
	//	value.SB_VP.BindVS(1);
	//
	//	// モデルデータ描画(影つけ)
	//	for (int i = 0; i < value.pModelData->GetMeshNum(); ++i)
	//	{
	//		ShaderManager::GetInstance().SetTexturePS(value.pModelData[i].GetTexture(i));
	//		value.pModelBuf[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	}
	//
	//	value.SB_World = XMMatrixTranspose(DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f) * DirectX::XMMatrixTranslation(1.f, 0.f, 1.f));
	//	value.SB_World.BindVS(0);
	//	for (int i = 0; i < value.pModelData->GetMeshNum(); ++i)
	//	{
	//		ShaderManager::GetInstance().SetTexturePS(value.pModelData[i].GetTexture(i));
	//		value.pModelBuf[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	}
	//}
	// 元の描画先に戻す
	//D3DClass::GetInstance().SetRenderTarget(viewW, viewH, nullptr, nullptr);

	if (this == m_pLight) {
		SHADER_LIGHT buf;
		buf.vLightDir = XMLoadFloat3(&m_pLight->m_direction);
		buf.vLd = XMLoadFloat4(&m_pLight->m_diffuse);
		buf.vLa = XMLoadFloat4(&m_pLight->m_ambient);
		buf.vLs = XMLoadFloat4(&m_pLight->m_specular);
		ShaderManager::GetInstance().UpdateBuffer("MainLight", &buf);
	}
}


// EOF