/**
 * @file GameObjectUI.cpp
 */


/**
 * @include
 */
#include "GameObjectText.h"
#include "D3DClass.h"
#include "polygon.h"
#include "Geometory.h"
#include "Material.h"
#include "Text.h"
#include "System.h"


GameObjectText::GameObjectText(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	m_layer(layer), m_texture(texture), GameObject(name, tag) {
	// 変数の初期化
	m_transform->m_scale = { 100.f, 100.f, 0 };
	// テクスチャ設定
	m_texPattern = float3(0.f, 0.f, 0.f);
	m_texSize = float3(1.f, 1.f, 1.f);
	// マテリアル
	m_material = AddComponent<Material>();
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値はテクスチャrgbはモデル自体の色
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値を０にすると真っ白 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 値を小さくするとモデルが薄くなる
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// 光沢
	// シェーダ
	m_vs = VS_2D;
	m_ps = PS_2D;


	// テキスト
	m_szText[0] = '\0';
	m_fontSize[0] = 16;
	m_fontSize[1] = 16;
	SetFontSize(16.f);
}


GameObjectText::~GameObjectText() {}


void GameObjectText::Init() {
	GameObject::Init();
}


void GameObjectText::Uninit() {
	GameObject::Uninit();
}


void GameObjectText::Update() {
	GameObject::Update();
}


void GameObjectText::LastUpdate() {
	GameObject::LastUpdate();
}


void GameObjectText::Draw() {

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
	world.mWorld	= XMMatrixTranspose(XMLoadFloat4x4(&m_transform->GetMatrix()));
	world.mTexture	= XMMatrixTranspose(mtxTex);
	shader->UpdateBuffer("MainWorld", &world);
	// マテリアル
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// テクスチャの反映
	shader->SetTexturePS(TextureManager::GetInstance().Get(m_texture));

	// 描画
	DrawPolygon();
	GameObject::Draw();

	// テキストの初期化
	m_szText[0] = '\0';
}



void GameObjectText::SetText(const char *fmt, ...) {
	va_list list;
	char aBuf[256];

	va_start(list, fmt);
	_vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// 連結
	if ((strlen(m_szText) + strlen(aBuf)) < sizeof(m_szText) - 1) {
		strcat_s(m_szText, sizeof(m_szText), aBuf);
	}
}

void GameObjectText::SetFontSize(float size) {
	SetFontSize(size, size);
}

void GameObjectText::SetFontSize(float w, float h) {
	m_fontSize[0] = w;
	m_fontSize[1] = h;
}


// EOF