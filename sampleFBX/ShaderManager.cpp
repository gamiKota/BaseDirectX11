#include "ShaderManager.h"
#include "ShaderBuffer.h"
#include "Camera.h"
#include "Transform.h"
#include "D3DClass.h"
#include "System.h"

// 実行エラー
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
static_assert(!(VS_MAX < _countof(pVSPath)), "E_VSへの定義追加忘れ");
static_assert(!(VS_MAX > _countof(pVSPath)), "VSPathへの読込ファイル追加忘れ");
static_assert(VS_MAX == _countof(Layouts), "頂点シェーダレイアウト指定忘れ");
const char* pPSPath[] =
{
	"data/shader/PixelShader.cso",
	"data/shader/Pixel2D.cso",
	"data/shader/ColorPS.cso",
	"data/shader/PhongPS.cso",
	"data/shader/HPGaugePS.cso",
	"data/shader/AreaWallPS.cso",
};
static_assert(!(PS_MAX < _countof(pPSPath)), "E_PSへの定義追加忘れ");
static_assert(!(PS_MAX > _countof(pPSPath)), L"PSPathへの読込ファイル追加忘れ");
const char* pGSPath[] =
{
	"data/shader/GeometryShader.cso",
	"data/shader/LineGS.cso",
};
static_assert(!(GS_MAX < _countof(pGSPath)), "E_GSへの定義追加忘れ");
static_assert(!(GS_MAX > _countof(pGSPath)), L"GSPathへの読込ファイル追加忘れ");


using namespace DirectX;


void ShaderManager::Initialize() {

	HRESULT hr;

	m_vsList.resize(VS_MAX);
	for (int i = 0; i < VS_MAX; ++i) {
		m_vsList[i] = new VertexShader(Layouts[i]);
		hr = m_vsList[i]->Create(pVSPath[i]);
		FnAssert(hr, "頂点シェーダ読み込み失敗");
	}
	m_psList.resize(PS_MAX);
	for (int i = 0; i < PS_MAX; ++i) {
		m_psList[i] = new PixelShader();
		hr = m_psList[i]->Create(pPSPath[i]);
		FnAssert(hr, "ピクセルシェーダ読み込み失敗");
	}
	m_gsList.resize(GS_MAX);
	for (int i = 0; i < GS_MAX; ++i) {
		m_gsList[i] = new GeometryShader();
		hr = m_gsList[i]->Create(pGSPath[i]);
		FnAssert(hr, "ジオメトリシェーダ読み込み失敗");
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