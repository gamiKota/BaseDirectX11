/**
 * @file GameObject3D.cpp
 */


/**
 * @include
 */
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "D3DClass.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderManager.h"
#include "System.h"


GameObject3D::GameObject3D(E_MODEL m_model, std::string name, std::string tag) : 
	m_model(m_model)
	, m_isLight(true)
	, m_isInstancing(false)
	, GameObject(name, tag, E_OBJECT::MODEL)
{
	m_transform->m_scale = { 0.5f, 0.5f, 0.5f };
	m_material = AddComponent<Material>();
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値はテクスチャrgbはモデル自体の色
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a値を０にすると真っ白 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 値を小さくするとモデルが薄くなる
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// 光沢

	m_vs = VS_NORMAL;
	m_ps = PS_NORMAL;
}


GameObject3D::~GameObject3D() {
}


void GameObject3D::Init() {
	GameObject::Init();
}


void GameObject3D::Uninit() {
	GameObject::Uninit();
}


void GameObject3D::Update() {
	GameObject::Update();
}


void GameObject3D::LastUpdate() {
	GameObject::LastUpdate();
	ModelManager::GetInstance().Update(m_model);
}


void GameObject3D::Draw() {


	if (m_isInstancing) {
		INSTANCING_PARAMETER ip;
		ip.mWorld = XMLoadFloat4x4(&m_transform->GetMatrix());
		ModelManager::GetInstance().SetInstancingParamter(ip);
	}
	else {
		ShaderManager* shader = &ShaderManager::GetInstance();

		// シェーダの適用
		shader->BindVS(m_vs);
		shader->BindPS(m_ps);
		shader->BindGS(GS_NORMAL);

		// シェーダの設定
		// ライト
		SHADER_LIGHT_SETTING light;
		light.light = (m_isLight) ? XMFLOAT4(1.f, 1.f, 1.f, 1.f) : XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		shader->UpdateBuffer("MainLightSetting", &light);
		// ワールド行列
		SHADER_WORLD world;
		world.mWorld = world.mWorldInstancing[0] = XMMatrixTranspose(XMLoadFloat4x4(&m_transform->GetMatrix()));
		shader->UpdateBuffer("MainWorld", &world);
		// マテリアル
		SHADER_MATERIAL material;
		material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
		material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
		material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
		material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
		shader->UpdateBuffer("Material", &material);

		// モデルデータ描画
		ModelManager::GetInstance().Draw(m_model);
	}
	// デバック表示
	for (auto com : m_listComponent) {
		Collision* col = dynamic_cast<Collision*>(com);
		if (col) {
			col->DebugDraw();
		}
	}
	//GameObject::Draw();
}


// EOF