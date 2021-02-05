/**
 * @file GameObjectUI.cpp
 */


/**
 * @include
 */
#include "GameObjectUI.h"
#include "D3DClass.h"
#include "polygon.h"
#include "Geometory.h"
#include "System.h"


GameObjectUI::GameObjectUI(E_LAYER layer) : m_layer(layer), m_texture(E_TEXTURE_NONE), GameObject("GameObjectUI") {

}


GameObjectUI::GameObjectUI(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	m_layer(layer), m_texture(texture), GameObject(name, tag) {
	// 変数の初期化
	m_transform->m_scale = { 100.f, 100.f, 0 };
	m_color = float3(1.f, 1.f, 1.f);
	m_alpha = 1.f;

	// テクスチャ設定
	m_texPattern = float3(0.f, 0.f, 0.f);
	m_texSize = float3(1.f, 1.f, 1.f);

	m_vs = VS_2D;
	m_ps = PS_2D;
}


GameObjectUI::~GameObjectUI() {
}


void GameObjectUI::Init() {
	GameObject::Init();
}


void GameObjectUI::Uninit() {
	GameObject::Uninit();
}


void GameObjectUI::Update() {
	GameObject::Update();
}


void GameObjectUI::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectUI::Draw() {

	ShaderManager* shader = &ShaderManager::GetInstance();
	ID3D11DeviceContext* DC = D3DClass::GetInstance().GetDeviceContext();

	shader->BindVS(m_vs);
	shader->BindPS(m_ps);
	DC->HSSetShader(NULL, NULL, 0);
	DC->DSSetShader(NULL, NULL, 0);
	DC->GSSetShader(NULL, NULL, 0);
	DC->CSSetShader(NULL, NULL, 0);

	// テクスチャマトリックスの初期化
	XMMATRIX mtxTex, mtxScale, mtxTranslate;
	mtxTex = XMMatrixIdentity();
	// スケールを反映
	mtxScale = XMMatrixScaling(m_texSize.x, m_texSize.y, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex, mtxScale);
	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_texSize.x * m_texPattern.x, m_texSize.y * m_texPattern.y, 0.0f);
	mtxTex = XMMatrixMultiply(mtxTex, mtxTranslate);

	// 行列情報
	SHADER_WORLD world;
	world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_transform->GetMatrix()));
	world.mTexture = XMMatrixTranspose(mtxTex);
	shader->UpdateBuffer("MainWorld", &world);

	// テクスチャの反映
	shader->SetTexturePS(TextureManager::GetInstance().Get(m_texture));

	// 描画
	DrawPolygon();
	GameObject::Draw();
}


bool GameObjectUISort(GameObject* obj1, GameObject* obj2) {
	GameObjectUI* temp1 = dynamic_cast<GameObjectUI*>(obj1);
	GameObjectUI* temp2 = dynamic_cast<GameObjectUI*>(obj2);
	if (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->m_layer < temp2->m_layer) {
			return true;
		}
	}
	return false;
}


// EOF