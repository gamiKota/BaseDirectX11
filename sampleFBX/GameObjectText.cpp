/**
 * @file GameObjectUI.cpp
 */


/**
 * @include
 */
#include "GameObjectText.h"
#include "D3DClass.h"
#include "Geometory.h"
#include "Material.h"
#include "Transform.h"
#include "System.h"


static XMFLOAT4X4					g_mWorld;				// ���[���h�ϊ��s��
static XMFLOAT4X4					g_mTex;					// �e�N�X�`���ϊ��s��


GameObjectText::GameObjectText(E_LAYER layer, E_TEXTURE texture, std::string name, std::string tag) :
	GameObjectUI(layer, texture, name, tag) {
	// �ϐ��̏�����
	m_transform->m_scale = { 100.f, 100.f, 0 };
	// �e�N�X�`���ݒ�
	m_texPattern = float3(0.f, 0.f, 0.f);
	m_texSize = float3(1.f, 1.f, 1.f);
	// �}�e���A��
	m_material = AddComponent<Material>();
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l�̓e�N�X�`��rgb�̓��f�����̂̐F
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l���O�ɂ���Ɛ^���� 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// �l������������ƃ��f���������Ȃ�
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// ����
	// �V�F�[�_
	m_vs = VS_2D;
	m_ps = PS_2D;

	// �e�L�X�g
	m_szText[0] = '\0';
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

	// �}�e���A��
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// �e�N�X�`���̔��f
	shader->SetTexturePS(TextureManager::GetInstance().Get(m_texture));

	XMFLOAT2 vPos(m_transform->m_position.x, m_transform->m_position.y);

	for (char* pChr = &m_szText[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = m_transform->m_position.x;
			vPos.y -= m_fontSize[1];
			continue;
		}
		int nChr = (BYTE)*pChr;

		// ���[���h�s��
		XMMATRIX mWorld = XMMatrixScaling(m_fontSize[0], m_fontSize[1], 0.f);
		mWorld *= XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f);
		mWorld *= XMMatrixTranslation(vPos.x, vPos.y, 0.f);
		XMStoreFloat4x4(&g_mWorld, mWorld);
		// �e�N�X�`���s��
		mWorld = XMMatrixScaling(8.0f / 128.0f, 8.0f / 128.0f, 1.0f);
		mWorld *= XMMatrixTranslation((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f, 0.0f);
		XMStoreFloat4x4(&g_mTex, mWorld);
		// �萔�o�b�t�@�֊i�[
		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
		world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
		shader->UpdateBuffer("MainWorld", &world);
		// �|���S���̕`��
		DrawPolygon();

		vPos.x += m_fontSize[0];
	}

	GameObject::Draw();

	// �e�L�X�g�̏�����
	m_szText[0] = '\0';
}



void GameObjectText::SetText(const char *fmt, ...) {
	va_list list;
	char aBuf[256];

	va_start(list, fmt);
	_vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// �A��
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