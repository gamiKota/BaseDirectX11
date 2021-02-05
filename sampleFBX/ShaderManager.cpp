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
	"data/shader/Vertex2D.cso",
	"data/shader/MeshVS.cso"
};
VertexShader::Layout Layouts[] =
{
	VertexShader::LAYOUT_PCUN,
	VertexShader::LAYOUT_PCU,
	VertexShader::LAYOUT_PCU,
};
static_assert(!(VS_MAX < _countof(pVSPath)), "E_VS�ւ̒�`�ǉ��Y��");
static_assert(!(VS_MAX > _countof(pVSPath)), "VSPath�ւ̓Ǎ��t�@�C���ǉ��Y��");
static_assert(VS_MAX == _countof(Layouts), "���_�V�F�[�_���C�A�E�g�w��Y��");
const char* pPSPath[] =
{
	"data/shader/PixelShader.cso",
	"data/shader/Pixel2D.cso",
	"data/shader/ColorPS.cso",
	"data/shader/PhongPS.cso",
	"data/shader/HPGaugePS.cso",
	"data/shader/AreaWallPS.cso",
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

	m_vsList.resize(VS_MAX);
	for (int i = 0; i < VS_MAX; ++i) {
		m_vsList[i] = new VertexShader(Layouts[i]);
		hr = m_vsList[i]->Create(pVSPath[i]);
		FnAssert(hr, "���_�V�F�[�_�ǂݍ��ݎ��s");
	}
	m_psList.resize(PS_MAX);
	for (int i = 0; i < PS_MAX; ++i) {
		m_psList[i] = new PixelShader();
		hr = m_psList[i]->Create(pPSPath[i]);
		FnAssert(hr, "�s�N�Z���V�F�[�_�ǂݍ��ݎ��s");
	}
	m_gsList.resize(GS_MAX);
	for (int i = 0; i < GS_MAX; ++i) {
		m_gsList[i] = new GeometryShader();
		hr = m_gsList[i]->Create(pGSPath[i]);
		FnAssert(hr, "�W�I���g���V�F�[�_�ǂݍ��ݎ��s");
	}
}


void ShaderManager::Terminate() {
	for (int i = 0; i < PS_MAX; ++i) { SAFE_DELETE(m_psList[i]) }
	for (int i = 0; i < GS_MAX; ++i) { SAFE_DELETE(m_gsList[i]) }
	for (int i = 0; i < VS_MAX; ++i) { SAFE_DELETE(m_vsList[i]) }
}


void ShaderManager::BindPS(E_PS shader) {
	m_psList[shader]->Bind();
}


void ShaderManager::BindVS(E_VS shader) {
	m_vsList[shader]->Bind();
}

void ShaderManager::BindGS(E_GS shader) {
	m_gsList[shader]->Bind();
}


void ShaderManager::UpdateBuffer(std::string bufName, void *data) {
	ShaderBufferManager::GetInstance().Update(bufName, data);
	ShaderBufferManager::GetInstance().Bind(bufName);
}


void ShaderManager::SetTexturePS(ID3D11ShaderResourceView* pTex, UINT slot) {
	D3DClass::GetInstance().GetDeviceContext()->PSSetShaderResources(slot, 1, &pTex);
}


// EOF