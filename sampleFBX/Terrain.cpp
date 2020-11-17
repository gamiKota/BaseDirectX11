// 地面クラス [Land.cpp]
#include "Land.h"
#include "Camera.h"
//#include "main.h"

using namespace DirectX;


// マクロ定義
#define PATH_MODELLAND	"data/model/land2.fbx"

// 外部グローバル変数 (model.cpp)
extern CFbxLight	g_light;		// 光源情報

// コンストラクタ
CLand::CLand() : m_pModel(nullptr),
	m_pVertex(nullptr), m_nVertex(0),
	m_pIndex(nullptr), m_nIndex(0)
{
}

// デストラクタ
CLand::~CLand()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE(m_pModel);
}

// 初期化
HRESULT CLand::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// FBXファイルの読み込み
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE(m_pModel);
	m_pModel = new CFbxModel();
	hr = m_pModel->Init(pDevice, pDeviceContext, PATH_MODELLAND);
	if (SUCCEEDED(hr)) {
		m_pModel->SetCamera(CCamera::Get()->GetEye());
		m_pModel->SetLight(g_light);

		m_nVertex = m_pModel->GetVertexCount();
		m_pVertex = new TFbxVertex[m_nVertex];
		m_pModel->GetVertex(m_pVertex, m_nVertex);
		m_nIndex = m_pModel->GetIndexCount();
		m_pIndex = new int[m_nIndex];
		m_pModel->GetIndex(m_pIndex, m_nIndex);
	}
	return hr;
}

// 終了処理
void CLand::Uninit()
{
	// モデル削除
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE(m_pModel);
}

// 更新
void CLand::Update()
{
	// 何もしない.
}

// 描画
void CLand::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// FBXファイル表示
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixIdentity());
	CCamera* pCamera = CCamera::Get();
	m_pModel->Render(world, pCamera->GetView(),
		pCamera->GetProj(), eOpacityOnly);
}

// レイとの当たり判定
bool CLand::Collision(XMFLOAT3* pP0, XMFLOAT3* pW,
	XMFLOAT3* pX, XMFLOAT3* pN)
{
	for (int i = 0; i < m_nIndex; i += 3) {
		// 三角形の頂点 (反時計回りを時計回りに)
		XMFLOAT3& P1 = m_pVertex[m_pIndex[i + 2]].vPos;
		XMFLOAT3& P2 = m_pVertex[m_pIndex[i + 1]].vPos;
		XMFLOAT3& P3 = m_pVertex[m_pIndex[i + 0]].vPos;
		// 辺に対応するベクトル
		XMFLOAT3 P1P2(P2.x - P1.x, P2.y - P1.y,
			P2.z - P1.z);
		XMFLOAT3 P2P3(P3.x - P2.x, P3.y - P2.y,
			P3.z - P2.z);
		XMFLOAT3 P3P1(P1.x - P3.x, P1.y - P3.y,
			P1.z - P3.z);
		// 法線ベクトル算出
		XMVECTOR N = XMVector3Normalize(
			XMVector3Cross(XMLoadFloat3(&P1P2),
				XMLoadFloat3(&P2P3)));
		// 分母を求める [t=(N・(P1-P0)/(N・W)]
		float base;
		XMStoreFloat(&base, XMVector3Dot(
			N, XMLoadFloat3(pW)));
		if (base == 0.0f) {
			continue;	// 面とレイが平行
		}
		// 分子を求める
		XMFLOAT3 P0P1(P1.x - pP0->x,
			P1.y - pP0->y, P1.z - pP0->z);
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(N,
			XMLoadFloat3(&P0P1)));
		// 媒介変数tより、交点を求める
		float t = dot / base;
		if (t < 0.0f) {
			continue;	// 始点の反対(後ろ)に交点
		}
		XMFLOAT3 X(pP0->x + t * pW->x,
				   pP0->y + t * pW->y,
				   pP0->z + t * pW->z);
		// 内外判定
		XMFLOAT3 P1X(X.x - P1.x, X.y - P1.y,
			X.z - P1.z);
		XMStoreFloat(&dot, XMVector3Dot(N,
		XMVector3Cross(XMLoadFloat3(&P1P2),
			XMLoadFloat3(&P1X))));
		if (dot < 0.0f) {
			continue;
		}
		XMFLOAT3 P2X(X.x - P2.x, X.y - P2.y,
			X.z - P2.z);
		XMStoreFloat(&dot, XMVector3Dot(N,
			XMVector3Cross(XMLoadFloat3(&P2P3),
				XMLoadFloat3(&P2X))));
		if (dot < 0.0f) {
			continue;
		}
		XMFLOAT3 P3X(X.x - P3.x, X.y - P3.y,
			X.z - P3.z);
		XMStoreFloat(&dot, XMVector3Dot(N,
			XMVector3Cross(XMLoadFloat3(&P3P1),
				XMLoadFloat3(&P3X))));
		if (dot < 0.0f) {
			continue;
		}
		// 当たり
		*pX = X;
		if (pN) {
			XMStoreFloat3(pN, N);
		}
		return true;	// 交点あり
	}
	return false;		// 交点無し
}
