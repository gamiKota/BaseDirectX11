#include "ShaderManager.h"
#include "ShaderBuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "D3DClass.h"
#include "System.h"

// ���s�G���[
#define FnAssert(fn, str) do { _ASSERT_EXPR(SUCCEEDED(fn), _CRT_WIDE(str)); } while(0)

const char* pVSPath[] =
{
	"data/shader/VertexShader.cso",
	"data/shader/Vertex2D.cso"
};
VertexShader::Layout Layouts[] =
{
	VertexShader::LAYOUT_PCUN,
	VertexShader::LAYOUT_PCU,
};
static_assert(!(VS_MAX < _countof(pVSPath)), "E_VS�ւ̒�`�ǉ��Y��");
static_assert(!(VS_MAX > _countof(pVSPath)), "VSPath�ւ̓Ǎ��t�@�C���ǉ��Y��");
static_assert(VS_MAX == _countof(Layouts), "���_�V�F�[�_���C�A�E�g�w��Y��");
const char* pPSPath[] =
{
	"data/shader/PixelShader.cso",
	"data/shader/Pixel2D.cso",
};
static_assert(!(PS_MAX < _countof(pPSPath)), "E_PS�ւ̒�`�ǉ��Y��");
static_assert(!(PS_MAX > _countof(pPSPath)), L"PSPath�ւ̓Ǎ��t�@�C���ǉ��Y��");
const char* pGSPath[] =
{
	"data/shader/GeometryShader.cso",
	"data/shader/LineGS.cso",
};
static_assert(!(GS_MAX < _countof(pGSPath)), "E_GS�ւ̒�`�ǉ��Y��");
static_assert(!(GS_MAX > _countof(pGSPath)), L"GSPath�ւ̓Ǎ��t�@�C���ǉ��Y��");


using namespace DirectX;


void ShaderManager::Initialize() {

	HRESULT hr;

	for (int i = 0; i < VS_MAX; ++i) {
		m_VS[i] = new VertexShader(Layouts[i]);
		hr = m_VS[i]->Create(pVSPath[i]);
		FnAssert(hr, "���_�V�F�[�_�ǂݍ��ݎ��s");
	}
	for (int i = 0; i < PS_MAX; ++i) {
		m_PS[i] = new PixelShader();
		hr = m_PS[i]->Create(pPSPath[i]);
		FnAssert(hr, "�s�N�Z���V�F�[�_�ǂݍ��ݎ��s");
	}
	for (int i = 0; i < GS_MAX; ++i) {
		m_GS[i] = new GeometryShader();
		hr = m_GS[i]->Create(pGSPath[i]);
		FnAssert(hr, "�W�I���g���V�F�[�_�ǂݍ��ݎ��s");
	}
}


void ShaderManager::Terminate() {
	for (int i = 0; i < PS_MAX; ++i) { SAFE_DELETE(m_PS[i]) }
	for (int i = 0; i < GS_MAX; ++i) { SAFE_DELETE(m_GS[i]) }
	for (int i = 0; i < VS_MAX; ++i) { SAFE_DELETE(m_VS[i]) }
}


void ShaderManager::BindPS(E_PS shader) {
	m_PS[shader]->Bind();
}


void ShaderManager::BindVS(E_VS shader) {
	m_VS[shader]->Bind();
}

void ShaderManager::BindGS(E_GS shader) {
	m_GS[shader]->Bind();
}


void ShaderManager::UpdateBuffer(std::string bufName, void *data) {
	ShaderBufferManager::GetInstance().Update(bufName, data);
	ShaderBufferManager::GetInstance().Bind(bufName);
}


void ShaderManager::SetTexturePS(ID3D11ShaderResourceView* pTex, UINT slot) {
	D3DClass::GetInstance().GetDeviceContext()->PSSetShaderResources(slot, 1, &pTex);
}


// EOF