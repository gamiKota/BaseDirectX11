// �n�ʃN���X [Land.cpp]
#include "Land.h"
#include "Camera.h"
//#include "main.h"

using namespace DirectX;


// �}�N����`
#define PATH_MODELLAND	"data/model/land2.fbx"

// �O���O���[�o���ϐ� (model.cpp)
extern CFbxLight	g_light;		// �������

// �R���X�g���N�^
CLand::CLand() : m_pModel(nullptr),
	m_pVertex(nullptr), m_nVertex(0),
	m_pIndex(nullptr), m_nIndex(0)
{
}

// �f�X�g���N�^
CLand::~CLand()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE(m_pModel);
}

// ������
HRESULT CLand::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// FBX�t�@�C���̓ǂݍ���
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

// �I������
void CLand::Uninit()
{
	// ���f���폜
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE(m_pModel);
}

// �X�V
void CLand::Update()
{
	// �������Ȃ�.
}

// �`��
void CLand::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// FBX�t�@�C���\��
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixIdentity());
	CCamera* pCamera = CCamera::Get();
	m_pModel->Render(world, pCamera->GetView(),
		pCamera->GetProj(), eOpacityOnly);
}

// ���C�Ƃ̓����蔻��
bool CLand::Collision(XMFLOAT3* pP0, XMFLOAT3* pW,
	XMFLOAT3* pX, XMFLOAT3* pN)
{
	for (int i = 0; i < m_nIndex; i += 3) {
		// �O�p�`�̒��_ (�����v�������v����)
		XMFLOAT3& P1 = m_pVertex[m_pIndex[i + 2]].vPos;
		XMFLOAT3& P2 = m_pVertex[m_pIndex[i + 1]].vPos;
		XMFLOAT3& P3 = m_pVertex[m_pIndex[i + 0]].vPos;
		// �ӂɑΉ�����x�N�g��
		XMFLOAT3 P1P2(P2.x - P1.x, P2.y - P1.y,
			P2.z - P1.z);
		XMFLOAT3 P2P3(P3.x - P2.x, P3.y - P2.y,
			P3.z - P2.z);
		XMFLOAT3 P3P1(P1.x - P3.x, P1.y - P3.y,
			P1.z - P3.z);
		// �@���x�N�g���Z�o
		XMVECTOR N = XMVector3Normalize(
			XMVector3Cross(XMLoadFloat3(&P1P2),
				XMLoadFloat3(&P2P3)));
		// ��������߂� [t=(N�E(P1-P0)/(N�EW)]
		float base;
		XMStoreFloat(&base, XMVector3Dot(
			N, XMLoadFloat3(pW)));
		if (base == 0.0f) {
			continue;	// �ʂƃ��C�����s
		}
		// ���q�����߂�
		XMFLOAT3 P0P1(P1.x - pP0->x,
			P1.y - pP0->y, P1.z - pP0->z);
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(N,
			XMLoadFloat3(&P0P1)));
		// �}��ϐ�t���A��_�����߂�
		float t = dot / base;
		if (t < 0.0f) {
			continue;	// �n�_�̔���(���)�Ɍ�_
		}
		XMFLOAT3 X(pP0->x + t * pW->x,
				   pP0->y + t * pW->y,
				   pP0->z + t * pW->z);
		// ���O����
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
		// ������
		*pX = X;
		if (pN) {
			XMStoreFloat3(pN, N);
		}
		return true;	// ��_����
	}
	return false;		// ��_����
}
