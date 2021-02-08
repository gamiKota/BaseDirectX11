#include "Text.h"
#include "Graphics.h"
#include "polygon.h"
#include <Windows.h>
#include <stdio.h>
#include "TextureManager.h"
#include "Material.h"
#include "ShaderManager.h"
#include "Geometory.h"
#include "System.h"

static XMFLOAT4X4					g_mWorld;				// ���[���h�ϊ��s��
static XMFLOAT4X4					g_mTex;					// �e�N�X�`���ϊ��s��

Text::Text() {
	m_szText[0] = '\0';
	m_fontSize[0] = 16;
	m_fontSize[1] = 16;

	m_material = new Material;
	m_material->m_ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l�̓e�N�X�`��rgb�̓��f�����̂̐F
	m_material->m_emissive	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// a�l���O�ɂ���Ɛ^���� 
	m_material->m_diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// �l������������ƃ��f���������Ȃ�
	m_material->m_specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);	// ����
}

Text::~Text() {
	delete m_material;
}

void Text::SetText(const char *fmt, ...) {
	va_list list;
	char aBuf[256];

	m_szText[0] = '\0';

	va_start(list, fmt);
	_vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// �A��
	if ((strlen(m_szText) + strlen(aBuf)) < sizeof(m_szText) - 1) {
		strcat_s(m_szText, sizeof(m_szText), aBuf);
	}
}

void Text::SetFontSize(float size) {
	SetFontSize(size, size);
}

void Text::SetFontSize(float w, float h) {
	m_fontSize[0] = w;
	m_fontSize[1] = h;
}

void Text::Bind() {

	ShaderManager* shader = &ShaderManager::GetInstance();
	ID3D11DeviceContext* pDeviceContext = D3DClass::GetInstance().GetDeviceContext();

	// �}�e���A��
	SHADER_MATERIAL material;
	material.vAmbient	= XMLoadFloat4(&m_material->m_ambient);
	material.vDiffuse	= XMLoadFloat4(&m_material->m_diffuse);
	material.vEmissive	= XMLoadFloat4(&m_material->m_emissive);
	material.vSpecular	= XMLoadFloat4(&m_material->m_specular);
	shader->UpdateBuffer("Material", &material);

	// �e�N�X�`���̃Z�b�g
	shader->SetTexturePS(TextureManager::GetInstance().Get(E_TEXTURE_TEXT));


	XMFLOAT2 vPos(SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f,
		SCREEN_HEIGHT * 0.5f - m_fontSize[1] * 0.5f);


	for (char* pChr = &m_szText[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f;
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
}



/*
if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + m_fontSize[0] * 0.5f;
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

		SHADER_WORLD world;
		world.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
		world.mTexture = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
		shader->UpdateBuffer("MainWorld", &world);

		DrawPolygon(pDeviceContext);

		vPos.x += m_fontSize[0];
*/


// EOF